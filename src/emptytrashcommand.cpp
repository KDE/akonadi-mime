/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "emptytrashcommand.h"
#include "util_p.h"
#include "imapsettings.h"
#include "specialmailcollections.h"

#include "akonadi_mime_debug.h"
#include <KLocalizedString>
#include <KMessageBox>

#include <entitytreemodel.h>
#include <itemfetchjob.h>
#include <itemdeletejob.h>
#include <agentmanager.h>
#include <kmime/kmime_message.h>
using namespace Akonadi;
EmptyTrashCommand::EmptyTrashCommand(const QAbstractItemModel *model, QObject *parent)
    : CommandBase(parent)
    , mModel(model)
    , the_trashCollectionFolder(-1)
    , mNumberOfTrashToEmpty(0)
{
}

EmptyTrashCommand::EmptyTrashCommand(const Akonadi::Collection &folder, QObject *parent)
    : CommandBase(parent)
    , mModel(nullptr)
    , the_trashCollectionFolder(-1)
    , mFolder(folder)
    , mNumberOfTrashToEmpty(0)
{
}

void EmptyTrashCommand::execute()
{
    if (!mFolder.isValid() && !mModel) {
        emitResult(Failed);
        return;
    }

    if (!mFolder.isValid()) {   //expunge all
        const QString title = i18n("Empty Trash");
        const QString text = i18n("Are you sure you want to empty the trash folders of all accounts?");
        if (KMessageBox::warningContinueCancel(nullptr, text, title,
                                               KStandardGuiItem::cont(), KStandardGuiItem::cancel(),
                                               QStringLiteral("confirm_empty_trash"))
            != KMessageBox::Continue) {
            emitResult(OK);
            return;
        }
        Akonadi::Collection trash = trashCollectionFolder();
        QVector<Akonadi::Collection> trashFolder;
        trashFolder << trash;

        const Akonadi::AgentInstance::List lst = agentInstances();
        for (const Akonadi::AgentInstance &type : lst) {
            if (type.identifier().contains(IMAP_RESOURCE_IDENTIFIER)) {
                if (type.status() == Akonadi::AgentInstance::Broken) {
                    continue;
                }
                QScopedPointer<OrgKdeAkonadiImapSettingsInterface> iface{Util::createImapSettingsInterface(type.identifier())};
                if (iface->isValid()) {
                    const int trashImap = iface->trashCollection();
                    if (trashImap != trash.id()) {
                        trashFolder << Akonadi::Collection(trashImap);
                    }
                }
            }
        }
        mNumberOfTrashToEmpty = trashFolder.count();
        for (int i = 0; i < mNumberOfTrashToEmpty; ++i) {
            expunge(trashFolder.at(i));
        }
    } else {
        if (folderIsTrash(mFolder)) {
            mNumberOfTrashToEmpty++;
            expunge(mFolder);
        } else {
            emitResult(OK);
        }
    }
}

void EmptyTrashCommand::expunge(const Akonadi::Collection &col)
{
    if (col.isValid()) {
        Akonadi::ItemDeleteJob *jobDelete = new Akonadi::ItemDeleteJob(col, this);
        connect(jobDelete, &Akonadi::ItemDeleteJob::result,
                this, [this, jobDelete]() {
                    if (jobDelete->error()) {
                        Util::showJobError(jobDelete);
                        emitResult(Failed);
                    }
                    emitResult(OK);
                });
    } else {
        qCWarning(AKONADIMIME_LOG) << " Try to expunge an invalid collection :" << col;
        emitResult(Failed);
    }
}

Akonadi::AgentInstance::List EmptyTrashCommand::agentInstances()
{
    Akonadi::AgentInstance::List relevantInstances;
    const auto instances = Akonadi::AgentManager::self()->instances();
    for (const Akonadi::AgentInstance &instance : instances) {
        if (instance.type().mimeTypes().contains(KMime::Message::mimeType())
            && instance.type().capabilities().contains(QLatin1String("Resource"))
            && !instance.type().capabilities().contains(QLatin1String("Virtual"))) {
            relevantInstances << instance;
        }
    }
    return relevantInstances;
}

Akonadi::Collection EmptyTrashCommand::collectionFromId(Collection::Id id) const
{
    const QModelIndex idx = Akonadi::EntityTreeModel::modelIndexForCollection(
        mModel, Akonadi::Collection(id));
    return idx.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
}

Akonadi::Collection EmptyTrashCommand::trashCollectionFolder()
{
    if (the_trashCollectionFolder < 0) {
        the_trashCollectionFolder = Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Trash).id();
    }
    return collectionFromId(the_trashCollectionFolder);
}

bool EmptyTrashCommand::folderIsTrash(const Akonadi::Collection &col)
{
    if (col == Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Trash)) {
        return true;
    }
    const Akonadi::AgentInstance::List lst = agentInstances();
    for (const Akonadi::AgentInstance &type : lst) {
        if (type.status() == Akonadi::AgentInstance::Broken) {
            continue;
        }
        if (type.identifier().contains(IMAP_RESOURCE_IDENTIFIER)) {
            QScopedPointer<OrgKdeAkonadiImapSettingsInterface> iface{Util::createImapSettingsInterface(type.identifier())};
            if (iface->isValid()) {
                if (iface->trashCollection() == col.id()) {
                    return true;
                }
            }
        }
    }
    return false;
}

void EmptyTrashCommand::emitResult(Result value)
{
    Q_EMIT result(value);
    mNumberOfTrashToEmpty--;
    if (mNumberOfTrashToEmpty <= 0) {
        deleteLater();
    }
}

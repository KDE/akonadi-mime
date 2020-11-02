/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "movetotrashcommand.h"
#include "util_p.h"
#include "movecommand.h"
#include "imapsettings.h"
#include "specialmailcollections.h"

#include <itemfetchjob.h>
#include <itemfetchscope.h>
#include <entitytreemodel.h>
using namespace Akonadi;
MoveToTrashCommand::MoveToTrashCommand(const QAbstractItemModel *model, const Akonadi::Collection::List &folders, QObject *parent)
    : CommandBase(parent)
{
    the_trashCollectionFolder = -1;
    mFolders = folders;
    mModel = model;
    mFolderListJobCount = mFolders.size();
}

MoveToTrashCommand::MoveToTrashCommand(const QAbstractItemModel *model, const Akonadi::Item::List &msgList, QObject *parent)
    : CommandBase(parent)
{
    the_trashCollectionFolder = -1;
    mMessages = msgList;
    mModel = model;
    mFolderListJobCount = 0;
}

void MoveToTrashCommand::slotFetchDone(KJob *job)
{
    mFolderListJobCount--;

    if (job->error()) {
        // handle errors
        Util::showJobError(job);
        emitResult(Failed);
        return;
    }

    auto *fjob = static_cast<Akonadi::ItemFetchJob *>(job);

    mMessages = fjob->items();
    moveMessages();

    if (mFolderListJobCount > 0) {
        auto *job = new Akonadi::ItemFetchJob(mFolders[mFolderListJobCount - 1], parent());
        job->fetchScope().setAncestorRetrieval(Akonadi::ItemFetchScope::Parent);
        connect(job, &Akonadi::ItemFetchJob::result, this, &MoveToTrashCommand::slotFetchDone);
    }
}

void MoveToTrashCommand::execute()
{
    if (!mFolders.isEmpty()) {
        auto *job = new Akonadi::ItemFetchJob(mFolders[mFolderListJobCount - 1], parent());
        job->fetchScope().setAncestorRetrieval(Akonadi::ItemFetchScope::Parent);
        connect(job, &Akonadi::ItemFetchJob::result, this, &MoveToTrashCommand::slotFetchDone);
    } else if (!mMessages.isEmpty()) {
        mFolders << mMessages.first().parentCollection();
        moveMessages();
    } else {
        emitResult(OK);
    }
}

void MoveToTrashCommand::moveMessages()
{
    const Akonadi::Collection folder = mFolders.at(mFolderListJobCount);
    if (folder.isValid()) {
        MoveCommand *moveCommand = new MoveCommand(findTrashFolder(folder), mMessages, this);
        connect(moveCommand, &MoveCommand::result, this, &MoveToTrashCommand::slotMoveDone);
        moveCommand->execute();
    } else {
        emitResult(Failed);
    }
}

void MoveToTrashCommand::slotMoveDone(Result result)
{
    if (result == Failed) {
        emitResult(Failed);
    }
    if (mFolderListJobCount == 0 && result == OK) {
        emitResult(OK);
    }
}

Akonadi::Collection MoveToTrashCommand::collectionFromId(Akonadi::Collection::Id id) const
{
    const QModelIndex idx = Akonadi::EntityTreeModel::modelIndexForCollection(
        mModel, Akonadi::Collection(id));
    return idx.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
}

Akonadi::Collection MoveToTrashCommand::trashCollectionFromResource(const Akonadi::Collection &col)
{
    //NOTE(Andras): from kmail/kmkernel.cpp
    Akonadi::Collection trashCol;
    if (col.isValid()) {
        if (col.resource().contains(IMAP_RESOURCE_IDENTIFIER)) {
            //TODO: we really need some standard interface to query for special collections,
            //instead of relying on a resource's settings interface
            OrgKdeAkonadiImapSettingsInterface *iface = Util::createImapSettingsInterface(col.resource());
            if (iface->isValid()) {
                trashCol = Akonadi::Collection(iface->trashCollection());
                delete iface;
                return trashCol;
            }
            delete iface;
        }
    }
    return trashCol;
}

Akonadi::Collection MoveToTrashCommand::trashCollectionFolder()
{
    if (the_trashCollectionFolder < 0) {
        the_trashCollectionFolder = Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Trash).id();
    }
    return collectionFromId(the_trashCollectionFolder);
}

Akonadi::Collection MoveToTrashCommand::findTrashFolder(const Akonadi::Collection &folder)
{
    Akonadi::Collection col = trashCollectionFromResource(folder);
    if (!col.isValid()) {
        col = trashCollectionFolder();
    }
    if (folder != col) {
        return col;
    }
    return Akonadi::Collection();
}

/*
    SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "markascommandhelper_p.h"

#include "akonadi_mime_debug.h"
#include <Akonadi/ItemModifyJob>

using namespace Akonadi;
namespace
{
static int sNumberMaxElement = 500;
}
MarkAsCommandHelper::MarkAsCommandHelper(QObject *parent)
    : QObject{parent}
{
}

MarkAsCommandHelper::~MarkAsCommandHelper() = default;

void MarkAsCommandHelper::start()
{
    if (mItemsToModify.isEmpty()) {
        emitResult(Akonadi::CommandBase::OK);
        deleteLater();
    } else {
        mIndex = 0;
        modifyMessages();
    }
}

void MarkAsCommandHelper::modifyMessages()
{
    auto listElement = mItemsToModify.mid(mIndex, qMin(mIndex + sNumberMaxElement, mItemsToModify.count()));
    // qDebug() << " mIndex " << mIndex << "listElement . count  " << listElement.count();
    mIndex += sNumberMaxElement;
    auto modifyJob = new Akonadi::ItemModifyJob(listElement, this);
    modifyJob->setIgnorePayload(true);
    modifyJob->disableRevisionCheck();
    connect(modifyJob, &Akonadi::ItemModifyJob::result, this, &MarkAsCommandHelper::slotModifyItemDone);
}

const Akonadi::Item::List &MarkAsCommandHelper::itemsToModify() const
{
    return mItemsToModify;
}

void MarkAsCommandHelper::setItemsToModify(const Akonadi::Item::List &newItemsToModify)
{
    mItemsToModify = newItemsToModify;
}

void MarkAsCommandHelper::slotModifyItemDone(KJob *job)
{
    if (job && job->error()) {
        qCDebug(AKONADIMIME_LOG) << " Error trying to set item status:" << job->errorText();
        emitResult(Akonadi::CommandBase::Failed);
    }
    if (mIndex > mItemsToModify.count()) {
        qCDebug(AKONADIMIME_LOG) << " finish";
        emitResult(Akonadi::CommandBase::OK);
        deleteLater();
    } else {
        // Modify more messages
        modifyMessages();
    }
}

#include "moc_markascommandhelper_p.cpp"

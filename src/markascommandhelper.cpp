/*
    SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "markascommandhelper_p.h"

#include "akonadi_mime_debug.h"
#include <Akonadi/ItemModifyJob>

using namespace Akonadi;
MarkAsCommandHelper::MarkAsCommandHelper(QObject *parent)
    : QObject{parent}
{
}

MarkAsCommandHelper::~MarkAsCommandHelper() = default;

void MarkAsCommandHelper::start()
{
    if (!mItemsToModify.isEmpty()) {
        auto modifyJob = new Akonadi::ItemModifyJob(mItemsToModify, this);
        modifyJob->setIgnorePayload(true);
        modifyJob->disableRevisionCheck();
        connect(modifyJob, &Akonadi::ItemModifyJob::result, this, &MarkAsCommandHelper::slotModifyItemDone);
    } else {
        emitResult(Akonadi::CommandBase::OK);
        deleteLater();
    }
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
    } else {
        emitResult(Akonadi::CommandBase::OK);
    }
    deleteLater();
}

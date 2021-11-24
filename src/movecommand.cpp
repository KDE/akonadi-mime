/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "movecommand.h"
#include "util_p.h"

#include <Akonadi/ItemDeleteJob>
#include <Akonadi/ItemMoveJob>
using namespace Akonadi;

class Akonadi::MoveCommandPrivate
{
public:
    MoveCommandPrivate() = default;

    Akonadi::Collection mDestFolder;
    Akonadi::Item::List mMessages;
};

MoveCommand::MoveCommand(const Akonadi::Collection &destFolder, const Akonadi::Item::List &msgList, QObject *parent)
    : CommandBase(parent)
    , d(new Akonadi::MoveCommandPrivate())
{
    d->mDestFolder = destFolder;
    d->mMessages = msgList;
}

MoveCommand::~MoveCommand() = default;

void MoveCommand::execute()
{
    if (d->mMessages.isEmpty()) {
        emitResult(OK);
        return;
    }
    if (d->mDestFolder.isValid()) {
        auto job = new Akonadi::ItemMoveJob(d->mMessages, d->mDestFolder, this);
        connect(job, &Akonadi::ItemMoveJob::result, this, &MoveCommand::slotMoveResult);
    } else {
        auto job = new Akonadi::ItemDeleteJob(d->mMessages, this);
        connect(job, &Akonadi::ItemDeleteJob::result, this, &MoveCommand::slotMoveResult);
    }
}

void MoveCommand::slotMoveResult(KJob *job)
{
    if (job->error()) {
        // handle errors
        Util::showJobError(job);
        emitResult(Failed);
    } else {
        emitResult(OK);
    }
}

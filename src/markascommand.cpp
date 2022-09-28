/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "markascommand.h"
#include "markascommandhelper_p.h"
#include "util_p.h"
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/ItemModifyJob>

#include <KLocalizedString>
#include <KMessageBox>

using namespace Akonadi;

class Akonadi::MarkAsCommandPrivate
{
public:
    MarkAsCommandPrivate() = default;

    Akonadi::Collection::List mFolders;
    Akonadi::Item::List mMessages;
    Akonadi::MessageStatus mTargetStatus;
    int mMarkJobCount = 0;
    int mFolderListJobCount = 0;
    int mInvertMark = 0;
    bool mRecursive = false;
};

MarkAsCommand::MarkAsCommand(Akonadi::MessageStatus targetStatus, const Akonadi::Item::List &msgList, bool invert, QObject *parent)
    : CommandBase(parent)
    , d(new Akonadi::MarkAsCommandPrivate())
{
    d->mInvertMark = invert;
    d->mMessages = msgList;
    d->mTargetStatus = targetStatus;
    d->mFolderListJobCount = 0;
    d->mMarkJobCount = 0;
}

MarkAsCommand::MarkAsCommand(Akonadi::MessageStatus targetStatus, const Akonadi::Collection::List &folders, bool invert, bool recursive, QObject *parent)
    : CommandBase(parent)
    , d(new Akonadi::MarkAsCommandPrivate())
{
    d->mInvertMark = invert;
    d->mFolders = folders;
    d->mTargetStatus = targetStatus;
    d->mFolderListJobCount = d->mFolders.size();
    d->mMarkJobCount = 0;
    d->mRecursive = recursive;
}

MarkAsCommand::~MarkAsCommand() = default;

void MarkAsCommand::slotCollectionFetchDone(KJob *job)
{
    if (job->error()) {
        Util::showJobError(job);
        emitResult(Failed);
        return;
    }

    auto fjob = static_cast<Akonadi::CollectionFetchJob *>(job);
    d->mFolders += fjob->collections();
    d->mFolderListJobCount = d->mFolders.size();

    // We have the subtree now so act as if we were passed the collections in ctor
    d->mRecursive = false;
    execute();
}

void MarkAsCommand::slotFetchDone(KJob *job)
{
    d->mFolderListJobCount--;

    if (job->error()) {
        // handle errors
        Util::showJobError(job);
        emitResult(Failed);
        return;
    }

    auto fjob = static_cast<Akonadi::ItemFetchJob *>(job);
    d->mMessages.clear();
    const auto items = fjob->items();
    for (const Akonadi::Item &item : items) {
        Akonadi::MessageStatus status;
        status.setStatusFromFlags(item.flags());
        if (d->mInvertMark) {
            if (status & d->mTargetStatus) {
                d->mMessages.append(item);
            }
        } else if (!(status & d->mTargetStatus)) {
            d->mMessages.append(item);
        }
    }
    if (d->mMessages.empty()) {
        if (d->mFolderListJobCount == 0) {
            emitResult(OK);
            return;
        }
    } else {
        markMessages();
    }
    if (d->mFolderListJobCount > 0) {
        auto fetchJob = new Akonadi::ItemFetchJob(d->mFolders[d->mFolderListJobCount - 1], parent());
        fetchJob->fetchScope().setAncestorRetrieval(Akonadi::ItemFetchScope::Parent);
        connect(fetchJob, &Akonadi::ItemFetchJob::result, this, &MarkAsCommand::slotFetchDone);
    }
}

void MarkAsCommand::execute()
{
    if (d->mRecursive && !d->mFolders.isEmpty()) {
        if (KMessageBox::questionYesNo(qobject_cast<QWidget *>(parent()),
                                       i18n("Are you sure you want to mark all messages in this folder and all its subfolders?"),
                                       i18n("Mark All Recursively"),
                                       KGuiItem(i18nc("@action:button", "Mark All")),
                                       KStandardGuiItem::cancel())
            == KMessageBox::Yes) {
            auto job = new Akonadi::CollectionFetchJob(d->mFolders.constFirst());
            connect(job, &Akonadi::CollectionFetchJob::result, this, &MarkAsCommand::slotCollectionFetchDone);
        } else {
            emitResult(Canceled);
        }
    } else if (!d->mFolders.isEmpty()) {
        // yes, we go backwards, shouldn't matter
        auto job = new Akonadi::ItemFetchJob(d->mFolders[d->mFolderListJobCount - 1], parent());
        job->fetchScope().setAncestorRetrieval(Akonadi::ItemFetchScope::Parent);
        connect(job, &Akonadi::ItemFetchJob::result, this, &MarkAsCommand::slotFetchDone);
    } else if (!d->mMessages.isEmpty()) {
        d->mFolders << d->mMessages.first().parentCollection();
        markMessages();
    } else {
        emitResult(OK);
    }
}

void MarkAsCommand::markMessages()
{
    d->mMarkJobCount = 0;

    QSet<QByteArray> flags = d->mTargetStatus.statusFlags();
    Q_ASSERT(flags.size() == 1);
    Akonadi::Item::Flag flag;
    if (!flags.isEmpty()) {
        flag = *(flags.begin());
    }

    Akonadi::Item::List itemsToModify;
    for (const Akonadi::Item &it : std::as_const(d->mMessages)) {
        Akonadi::Item item(it);

        // be careful to only change the flags we want to change, not to overwrite them
        // otherwise ItemModifyJob will not do what we expect
        if (d->mInvertMark) {
            if (item.hasFlag(flag)) {
                item.clearFlag(flag);
                itemsToModify.push_back(item);
            }
        } else {
            if (!item.hasFlag(flag)) {
                item.setFlag(flag);
                itemsToModify.push_back(item);
            }
        }
    }

    d->mMarkJobCount++;
    if (itemsToModify.isEmpty()) {
        slotModifyItemDone(); // pretend we did something
    } else {
        auto helper = new Akonadi::MarkAsCommandHelper(this);
        helper->setItemsToModify(itemsToModify);
        connect(helper, &Akonadi::MarkAsCommandHelper::emitResult, this, &MarkAsCommand::slotHelperDone);
        helper->start();
    }
}

void MarkAsCommand::slotHelperDone(Akonadi::CommandBase::Result result)
{
    d->mMarkJobCount--;
    if (result == Akonadi::CommandBase::Failed) {
        emitResult(Failed);
    }
    slotModifyItemDone();
}

void MarkAsCommand::slotModifyItemDone()
{
    if (d->mMarkJobCount == 0 && d->mFolderListJobCount == 0) {
        emitResult(OK);
    }
}

/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>
    SPDX-FileCopyrightText: 2012 Dan Vrátil <dvratil@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "removeduplicatesjob.h"
#include "akonadi_mime_debug.h"
#include <itemdeletejob.h>
#include <itemfetchjob.h>
#include <itemfetchscope.h>

#include <kmime/kmime_message.h>

#include <KLocalizedString>

class Q_DECL_HIDDEN Akonadi::RemoveDuplicatesJob::Private
{
public:
    Private(RemoveDuplicatesJob *parent)
        : mParent(parent)
    {
    }

    void fetchItem()
    {
        Akonadi::Collection collection = mFolders.value(mJobCount - 1);
        qCDebug(AKONADIMIME_LOG) << "Processing collection" << collection.name() << "(" << collection.id() << ")";

        auto job = new Akonadi::ItemFetchJob(collection, mParent);
        job->fetchScope().setAncestorRetrieval(Akonadi::ItemFetchScope::Parent);
        job->fetchScope().fetchFullPayload();
        job->fetchScope().setIgnoreRetrievalErrors(true);
        mParent->connect(job, &ItemFetchJob::result, mParent, [this](KJob *job) {
            slotFetchDone(job);
        });
        mCurrentJob = job;

        Q_EMIT mParent->description(mParent, i18n("Retrieving items..."));
    }

    void slotFetchDone(KJob *job)
    {
        mJobCount--;
        if (job->error()) {
            mParent->setError(job->error());
            mParent->setErrorText(job->errorText());
            mParent->emitResult();
            return;
        }

        if (mKilled) {
            mParent->emitResult();
            return;
        }

        Q_EMIT mParent->description(mParent, i18n("Searching for duplicates..."));

        auto fjob = static_cast<Akonadi::ItemFetchJob *>(job);
        Akonadi::Item::List items = fjob->items();

        // find duplicate mails with the same messageid
        // if duplicates are found, check the content as well to be sure they are the same
        QMap<QByteArray, uint> messageIds;
        QMap<uint, QList<uint>> duplicates;
        QMap<uint, uint> bodyHashes;
        const int numberOfItems(items.size());
        for (int i = 0; i < numberOfItems; ++i) {
            Akonadi::Item item = items.at(i);
            if (item.hasPayload<KMime::Message::Ptr>()) {
                auto message = item.payload<KMime::Message::Ptr>();
                QByteArray idStr = message->messageID()->as7BitString(false);
                // TODO: Maybe do some more check in case of idStr.isEmpty()
                // like when the first message's body is different from the 2nd,
                // but the 2nd is the same as the 3rd, etc.
                // if ( !idStr.isEmpty() )
                {
                    if (messageIds.contains(idStr)) {
                        uint mainId = messageIds.value(idStr);
                        if (!bodyHashes.contains(mainId)) {
                            bodyHashes.insert(mainId, qHash(items.value(mainId).payload<KMime::Message::Ptr>()->encodedContent()));
                        }
                        uint hash = qHash(message->encodedContent());
                        qCDebug(AKONADIMIME_LOG) << idStr << bodyHashes.value(mainId) << hash;
                        if (bodyHashes.value(mainId) == hash) {
                            duplicates[mainId].append(i);
                        }
                    } else {
                        messageIds.insert(idStr, i);
                    }
                }
            }
        }

        QMap<uint, QList<uint>>::ConstIterator end(duplicates.constEnd());
        for (QMap<uint, QList<uint>>::ConstIterator it = duplicates.constBegin(); it != end; ++it) {
            QList<uint>::ConstIterator dupEnd(it.value().constEnd());
            for (QList<uint>::ConstIterator dupIt = it.value().constBegin(); dupIt != dupEnd; ++dupIt) {
                mDuplicateItems.append(items.value(*dupIt));
            }
        }

        if (mKilled) {
            mParent->emitResult();
            return;
        }

        if (mJobCount > 0) {
            fetchItem();
        } else {
            if (mDuplicateItems.isEmpty()) {
                qCDebug(AKONADIMIME_LOG) << "No duplicates, I'm done here";
                mParent->emitResult();
                return;
            } else {
                Q_EMIT mParent->description(mParent, i18n("Removing duplicates..."));
                auto delCmd = new Akonadi::ItemDeleteJob(mDuplicateItems, mParent);
                mParent->connect(delCmd, &ItemDeleteJob::result, mParent, [this](KJob *job) {
                    slotDeleteDone(job);
                });
            }
        }
    }

    void slotDeleteDone(KJob *job)
    {
        qCDebug(AKONADIMIME_LOG) << "Job done";

        mParent->setError(job->error());
        mParent->setErrorText(job->errorText());
        mParent->emitResult();
    }

    Akonadi::Collection::List mFolders;
    Akonadi::Item::List mDuplicateItems;
    Akonadi::Job *mCurrentJob = nullptr;
    int mJobCount = 0;
    bool mKilled = false;

private:
    RemoveDuplicatesJob *const mParent;
};

using namespace Akonadi;

RemoveDuplicatesJob::RemoveDuplicatesJob(const Akonadi::Collection &folder, QObject *parent)
    : Job(parent)
    , d(new Private(this))
{
    d->mJobCount = 1;
    d->mFolders << folder;
}

RemoveDuplicatesJob::RemoveDuplicatesJob(const Akonadi::Collection::List &folders, QObject *parent)
    : Job(parent)
    , d(new Private(this))
{
    d->mFolders = folders;
    d->mJobCount = d->mFolders.length();
}

RemoveDuplicatesJob::~RemoveDuplicatesJob()
{
    delete d;
}

void RemoveDuplicatesJob::doStart()
{
    qCDebug(AKONADIMIME_LOG) << " void RemoveDuplicatesJob::doStart()";

    if (d->mFolders.isEmpty()) {
        qCWarning(AKONADIMIME_LOG) << "No collections to process";
        emitResult();
        return;
    }

    d->fetchItem();
}

bool RemoveDuplicatesJob::doKill()
{
    qCDebug(AKONADIMIME_LOG) << "Killed!";

    d->mKilled = true;
    if (d->mCurrentJob) {
        d->mCurrentJob->kill(EmitResult);
    }

    return true;
}

#include "moc_removeduplicatesjob.cpp"

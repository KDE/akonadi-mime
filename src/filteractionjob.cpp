/*
  SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "filteractionjob_p.h"

#include <Akonadi/Collection>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>

#include "akonadi_mime_debug.h"

using namespace Akonadi;

class Akonadi::FilterActionJobPrivate
{
public:
    explicit FilterActionJobPrivate(FilterActionJob *qq)
        : q(qq)
    {
    }

    ~FilterActionJobPrivate()
    {
        delete functor;
    }

    FilterActionJob *const q;
    Collection collection;
    Item::List items;
    FilterAction *functor = nullptr;
    ItemFetchScope fetchScope;

    // Q_SLOTS:
    void fetchResult(KJob *job);

    void traverseItems();
};

void FilterActionJobPrivate::fetchResult(KJob *job)
{
    if (job->error()) {
        // KCompositeJob takes care of errors.
        return;
    }

    auto fjob = qobject_cast<ItemFetchJob *>(job);
    Q_ASSERT(fjob);
    Q_ASSERT(items.isEmpty());
    items = fjob->items();
    traverseItems();
}

void FilterActionJobPrivate::traverseItems()
{
    Q_ASSERT(functor);
    qCDebug(AKONADIMIME_LOG) << "Traversing" << items.count() << "items.";
    for (const Item &item : std::as_const(items)) {
        if (functor->itemAccepted(item)) {
            functor->itemAction(item, q);
            qCDebug(AKONADIMIME_LOG) << "Added subjob for item" << item.id();
        }
    }
    if (q->subjobs().isEmpty()) {
        qCDebug(AKONADIMIME_LOG) << "No subjobs; I am done";
    } else {
        qCDebug(AKONADIMIME_LOG) << "Have subjobs; Done when last of them is";
    }
    q->commit();
}

FilterAction::~FilterAction() = default;

FilterActionJob::FilterActionJob(const Item &item, FilterAction *functor, QObject *parent)
    : TransactionSequence(parent)
    , d(new FilterActionJobPrivate(this))
{
    d->functor = functor;
    d->items << item;
}

FilterActionJob::FilterActionJob(const Item::List &items, FilterAction *functor, QObject *parent)
    : TransactionSequence(parent)
    , d(new FilterActionJobPrivate(this))
{
    d->functor = functor;
    d->items = items;
}

FilterActionJob::FilterActionJob(const Collection &collection, FilterAction *functor, QObject *parent)
    : TransactionSequence(parent)
    , d(new FilterActionJobPrivate(this))
{
    d->functor = functor;
    Q_ASSERT(collection.isValid());
    d->collection = collection;
}

FilterActionJob::~FilterActionJob() = default;

void FilterActionJob::doStart()
{
    if (d->collection.isValid()) {
        qCDebug(AKONADIMIME_LOG) << "Fetching collection" << d->collection.id();
        auto fjob = new ItemFetchJob(d->collection, this);
        Q_ASSERT(d->functor);
        d->fetchScope = d->functor->fetchScope();
        fjob->setFetchScope(d->fetchScope);
        connect(fjob, &ItemFetchJob::result, this, [this](KJob *job) {
            d->fetchResult(job);
        });
    } else {
        d->traverseItems();
    }
}

#include "moc_filteractionjob_p.cpp"

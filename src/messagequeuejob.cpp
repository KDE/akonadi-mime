/*
  SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagequeuejob.h"

#include "transportattribute.h"

#include "akonadi_mime_debug.h"
#include <KLocalizedString>

#include <Akonadi/Item>
#include <Akonadi/ItemCreateJob>
#include <Akonadi/MessageFlags>
#include <Akonadi/SpecialMailCollections>
#include <Akonadi/SpecialMailCollectionsRequestJob>

using namespace Akonadi;
using namespace KMime;

/**
  @internal
*/
class Akonadi::MessageQueueJobPrivate
{
public:
    explicit MessageQueueJobPrivate(MessageQueueJob *qq)
        : q(qq)
    {
    }

    MessageQueueJob *const q;

    Message::Ptr message;
    TransportAttribute transportAttribute;
    DispatchModeAttribute dispatchModeAttribute;
    SentBehaviourAttribute sentBehaviourAttribute;
    SentActionAttribute sentActionAttribute;
    AddressAttribute addressAttribute;
    bool started = false;

    /**
      Returns true if this message has everything it needs and is ready to be
      sent.
    */
    [[nodiscard]] bool validate() const;

    // slot
    void outboxRequestResult(KJob *job);
};

bool MessageQueueJobPrivate::validate() const
{
    if (!message) {
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("Empty message."));
        q->emitResult();
        return false;
    }

    if ((addressAttribute.to().count() + addressAttribute.cc().count() + addressAttribute.bcc().count()) == 0) {
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("Message has no recipients."));
        q->emitResult();
        return false;
    }

    if (sentBehaviourAttribute.sentBehaviour() == SentBehaviourAttribute::MoveToCollection && !(sentBehaviourAttribute.moveToCollection().isValid())) {
        q->setError(KJob::UserDefinedError);
        q->setErrorText(i18n("Message has invalid sent-mail folder."));
        q->emitResult();
        return false;
    } else if (sentBehaviourAttribute.sentBehaviour() == SentBehaviourAttribute::MoveToDefaultSentCollection) {
        // TODO require SpecialMailCollections::SentMail here?
    }

    return true; // all ok
}

void MessageQueueJobPrivate::outboxRequestResult(KJob *job)
{
    Q_ASSERT(!started);
    started = true;

    if (job->error()) {
        qCritical() << "Failed to get the Outbox folder:" << job->error() << job->errorString();
        q->setError(job->error());
        q->emitResult();
        return;
    }

    if (!validate()) {
        // The error has been set; the result has been emitted.
        return;
    }

    auto requestJob = qobject_cast<SpecialMailCollectionsRequestJob *>(job);
    if (!requestJob) {
        return;
    }

    // Create item.
    Item item;
    item.setMimeType(QStringLiteral("message/rfc822"));
    item.setPayload<Message::Ptr>(message);

    // Set attributes.
    item.addAttribute(addressAttribute.clone());
    item.addAttribute(dispatchModeAttribute.clone());
    item.addAttribute(sentBehaviourAttribute.clone());
    item.addAttribute(sentActionAttribute.clone());
    item.addAttribute(transportAttribute.clone());

    Akonadi::MessageFlags::copyMessageFlags(*message, item);
    // Set flags.
    item.setFlag(Akonadi::MessageFlags::Queued);

    // Store the item in the outbox.
    const Collection collection = requestJob->collection();
    Q_ASSERT(collection.isValid());
    auto cjob = new ItemCreateJob(item, collection); // job autostarts
    q->addSubjob(cjob);
}

MessageQueueJob::MessageQueueJob(QObject *parent)
    : KCompositeJob(parent)
    , d(new MessageQueueJobPrivate(this))
{
}

MessageQueueJob::~MessageQueueJob() = default;

Message::Ptr MessageQueueJob::message() const
{
    return d->message;
}

DispatchModeAttribute &MessageQueueJob::dispatchModeAttribute()
{
    return d->dispatchModeAttribute;
}

AddressAttribute &MessageQueueJob::addressAttribute()
{
    return d->addressAttribute;
}

TransportAttribute &MessageQueueJob::transportAttribute()
{
    return d->transportAttribute;
}

SentBehaviourAttribute &MessageQueueJob::sentBehaviourAttribute()
{
    return d->sentBehaviourAttribute;
}

SentActionAttribute &MessageQueueJob::sentActionAttribute()
{
    return d->sentActionAttribute;
}

void MessageQueueJob::setMessage(const Message::Ptr &message)
{
    d->message = message;
}

void MessageQueueJob::start()
{
    auto rjob = new SpecialMailCollectionsRequestJob(this);
    rjob->requestDefaultCollection(SpecialMailCollections::Outbox);
    connect(rjob, &SpecialMailCollectionsRequestJob::result, this, [this](KJob *job) {
        d->outboxRequestResult(job);
    });
    rjob->start();
}

void MessageQueueJob::slotResult(KJob *job)
{
    // error handling
    KCompositeJob::slotResult(job);

    if (!error()) {
        emitResult();
    }
}

#include "moc_messagequeuejob.cpp"

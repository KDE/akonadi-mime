/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "dispatcherinterface.h"
#include "dispatcherinterface_p.h"

#include "outboxactions_p.h"

#include "akonadi_mime_debug.h"

#include "transportattribute.h"
#include <Akonadi/AgentManager>
#include <Akonadi/Collection>
#include <Akonadi/SpecialMailCollections>

using namespace Akonadi;

Q_GLOBAL_STATIC(DispatcherInterfacePrivate, sInstance)

void DispatcherInterfacePrivate::massModifyResult(KJob *job)
{
    // Nothing to do here, really.  If the job fails, the user can retry it.
    if (job->error()) {
        qCDebug(AKONADIMIME_LOG) << "failed" << job->errorString();
    } else {
        qCDebug(AKONADIMIME_LOG) << "succeeded.";
    }
}

DispatcherInterface::DispatcherInterface() = default;

AgentInstance DispatcherInterface::dispatcherInstance() const
{
    AgentInstance a = AgentManager::self()->instance(QStringLiteral("akonadi_maildispatcher_agent"));
    if (!a.isValid()) {
        qCWarning(AKONADIMIME_LOG) << "Could not get MDA instance.";
    }
    return a;
}

void DispatcherInterface::dispatchManually()
{
    Collection outbox = SpecialMailCollections::self()->defaultCollection(SpecialMailCollections::Outbox);
    if (!outbox.isValid()) {
        //    qCritical() << "Could not access Outbox.";
        return;
    }

    auto mjob = new FilterActionJob(outbox, new SendQueuedAction, sInstance);
    QObject::connect(mjob, &KJob::result, sInstance(), &DispatcherInterfacePrivate::massModifyResult);
}

void DispatcherInterface::retryDispatching()
{
    Collection outbox = SpecialMailCollections::self()->defaultCollection(SpecialMailCollections::Outbox);
    if (!outbox.isValid()) {
        //    qCritical() << "Could not access Outbox.";
        return;
    }

    auto mjob = new FilterActionJob(outbox, new ClearErrorAction, sInstance);
    QObject::connect(mjob, &KJob::result, sInstance(), &DispatcherInterfacePrivate::massModifyResult);
}

void DispatcherInterface::dispatchManualTransport(int transportId)
{
    Collection outbox = SpecialMailCollections::self()->defaultCollection(SpecialMailCollections::Outbox);
    if (!outbox.isValid()) {
        //    qCritical() << "Could not access Outbox.";
        return;
    }

    auto mjob = new FilterActionJob(outbox, new DispatchManualTransportAction(transportId), sInstance);
    QObject::connect(mjob, &KJob::result, sInstance(), &DispatcherInterfacePrivate::massModifyResult);
}

#include "moc_dispatcherinterface_p.cpp"

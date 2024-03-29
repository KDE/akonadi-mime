/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "outboxactions_p.h"

#include "akonadi_mime_debug.h"
#include "dispatchmodeattribute.h"
#include "errorattribute.h"

#include <Akonadi/ItemModifyJob>
#include <Akonadi/MessageFlags>

using namespace Akonadi;

SendQueuedAction::SendQueuedAction() = default;

SendQueuedAction::~SendQueuedAction() = default;

ItemFetchScope SendQueuedAction::fetchScope() const
{
    ItemFetchScope scope;
    scope.fetchFullPayload(false);
    scope.fetchAttribute<DispatchModeAttribute>();
    scope.fetchAttribute<ErrorAttribute>();
    scope.setCacheOnly(true);
    return scope;
}

bool SendQueuedAction::itemAccepted(const Item &item) const
{
    if (!item.hasAttribute<DispatchModeAttribute>()) {
        qCWarning(AKONADIMIME_LOG) << "Item doesn't have DispatchModeAttribute.";
        return false;
    }

    return item.attribute<DispatchModeAttribute>()->dispatchMode() == DispatchModeAttribute::Manual;
}

Job *SendQueuedAction::itemAction(const Item &item, FilterActionJob *parent) const
{
    Item cp = item;
    cp.addAttribute(new DispatchModeAttribute); // defaults to Automatic
    if (cp.hasAttribute<ErrorAttribute>()) {
        cp.removeAttribute<ErrorAttribute>();
        cp.clearFlag(Akonadi::MessageFlags::HasError);
    }
    return new ItemModifyJob(cp, parent);
}

ClearErrorAction::ClearErrorAction() = default;

ClearErrorAction::~ClearErrorAction() = default;

ItemFetchScope ClearErrorAction::fetchScope() const
{
    ItemFetchScope scope;
    scope.fetchFullPayload(false);
    scope.fetchAttribute<ErrorAttribute>();
    scope.setCacheOnly(true);
    return scope;
}

bool ClearErrorAction::itemAccepted(const Item &item) const
{
    return item.hasAttribute<ErrorAttribute>();
}

Job *ClearErrorAction::itemAction(const Item &item, FilterActionJob *parent) const
{
    Item cp = item;
    cp.removeAttribute<ErrorAttribute>();
    cp.clearFlag(Akonadi::MessageFlags::HasError);
    cp.setFlag(Akonadi::MessageFlags::Queued);
    return new ItemModifyJob(cp, parent);
}

DispatchManualTransportAction::DispatchManualTransportAction(int transportId)
    : mTransportId(transportId)
{
}

DispatchManualTransportAction::~DispatchManualTransportAction() = default;

ItemFetchScope DispatchManualTransportAction::fetchScope() const
{
    ItemFetchScope scope;
    scope.fetchFullPayload(false);
    scope.fetchAttribute<TransportAttribute>();
    scope.fetchAttribute<DispatchModeAttribute>();
    scope.setCacheOnly(true);
    return scope;
}

bool DispatchManualTransportAction::itemAccepted(const Item &item) const
{
    if (!item.hasAttribute<DispatchModeAttribute>()) {
        qCWarning(AKONADIMIME_LOG) << "Item doesn't have DispatchModeAttribute.";
        return false;
    }

    if (!item.hasAttribute<TransportAttribute>()) {
        qCWarning(AKONADIMIME_LOG) << "Item doesn't have TransportAttribute.";
        return false;
    }

    return item.attribute<DispatchModeAttribute>()->dispatchMode() == DispatchModeAttribute::Manual;
}

Job *DispatchManualTransportAction::itemAction(const Item &item, FilterActionJob *parent) const
{
    Item cp = item;
    cp.attribute<TransportAttribute>()->setTransportId(mTransportId);
    cp.removeAttribute<DispatchModeAttribute>();
    cp.addAttribute(new DispatchModeAttribute); // defaults to Automatic
    cp.setFlag(Akonadi::MessageFlags::Queued);
    return new ItemModifyJob(cp, parent);
}

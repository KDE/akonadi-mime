/*
    SPDX-FileCopyrightText: 2006 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemodel.h"
#include "messageparts.h"

#include <Akonadi/ItemFetchScope>
#include <Akonadi/Monitor>
#include <Akonadi/Session>

#include <KMime/Message>

#include <KLocalizedString>

#include <KFormat>
#include <QLocale>

#include <KLazyLocalizedString>
#include <array>
using MessagePtr = std::shared_ptr<KMime::Message>;

using namespace Akonadi;

namespace
{
struct HeaderName {
    const KLazyLocalizedString desc;
};
constexpr std::array<HeaderName, 5> headers = {{{kli18nc("@title:column, message (e.g. email) subject", "Subject")},
                                                {kli18nc("@title:column, sender of message (e.g. email)", "Sender")},
                                                {kli18nc("@title:column, receiver of message (e.g. email)", "Receiver")},
                                                {kli18nc("@title:column, message (e.g. email) timestamp", "Date")},
                                                {kli18nc("@title:column, message (e.g. email) size", "Size")}}};
}

MessageModel::MessageModel(Monitor *monitor, QObject *parent)
    : EntityTreeModel(monitor, parent)
{
    // Make sure we have an envelope
    monitor->itemFetchScope().fetchPayloadPart(MessagePart::Envelope);
    setCollectionFetchStrategy(FetchCollectionsMerged);
}

int MessageModel::entityColumnCount(HeaderGroup group) const
{
    if (group == EntityTreeModel::ItemListHeaders) {
        return headers.size();
    }

    return EntityTreeModel::entityColumnCount(group);
}

QVariant MessageModel::entityData(const Item &item, int column, int role) const
{
    if (!item.hasPayload<std::shared_ptr<const KMime::Message>>()) {
        return {};
    }

    const auto msg = item.payload<std::shared_ptr<const KMime::Message>>();
    if (role == Qt::DisplayRole) {
        switch (column) {
        case Subject:
            if (auto s = msg->subject()) {
                return s->asUnicodeString();
            } else {
                return i18nc("@label Alternative text when email subject is missing", "(No subject)");
            }
        case Sender:
            if (auto s = msg->from()) {
                return s->asUnicodeString();
            } else {
                return i18nc("@label Alternative text when email sender is missing", "(No sender)");
            }
        case Receiver:
            if (auto s = msg->to()) {
                return s->asUnicodeString();
            } else {
                return i18nc("@label Alternative text when email recipient is missing", "(No receiver)");
            }
        case Date:
            if (auto s = msg->date()) {
                return QLocale().toString(s->dateTime());
            } else {
                return i18nc("@label Alternative text when email date/time is missing", "(No date)");
            }
        case Size:
            if (item.size() == 0) {
                return i18nc("@label No size available", "-");
            } else {
                KFormat format;
                return format.formatByteSize(item.size());
            }
        default:
            return {};
        }
    } else if (role == Qt::EditRole) {
        switch (column) {
        case Subject: {
            const auto h = msg->subject();
            return h ? h->asUnicodeString() : QString();
        }
        case Sender: {
            const auto h = msg->from();
            return h ? h->asUnicodeString() : QString();
        }
        case Receiver: {
            const auto h = msg->to();
            return h ? h->asUnicodeString() : QString();
        }
        case Date: {
            const auto h = msg->date();
            return h ? h->dateTime() : QDateTime();
        }
        case Size:
            return item.size();
        default:
            return {};
        }
    }

    return EntityTreeModel::entityData(item, column, role);
}

QVariant MessageModel::entityHeaderData(int section, Qt::Orientation orientation, int role, HeaderGroup headerGroup) const
{
    if (section >= 0 && static_cast<std::size_t>(section) < headers.size()) {
        return KLocalizedString(headers[section].desc).toString();
    }

    return EntityTreeModel::entityHeaderData(section, orientation, role, headerGroup);
}

#include "moc_messagemodel.cpp"

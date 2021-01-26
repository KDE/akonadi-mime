/*
    SPDX-FileCopyrightText: 2006 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagemodel.h"
#include "messageparts.h"

#include <entitytreemodel.h>
#include <itemfetchscope.h>
#include <monitor.h>
#include <session.h>

#include <kmime/kmime_message.h>

#include <KLocalizedString>

#include <KFormat>
#include <QLocale>

#include <array>

using MessagePtr = KMime::Message::Ptr;

using namespace Akonadi;

namespace
{
struct HeaderName {
    const char *desc;
    const char *name;
};
constexpr std::array<HeaderName, 5> headers = {{{I18NC_NOOP("@title:column, message (e.g. email) subject", "Subject")},
                                                {I18NC_NOOP("@title:column, sender of message (e.g. email)", "Sender")},
                                                {I18NC_NOOP("@title:column, receiver of message (e.g. email)", "Receiver")},
                                                {I18NC_NOOP("@title:column, message (e.g. email) timestamp", "Date")},
                                                {I18NC_NOOP("@title:column, message (e.g. email) size", "Size")}}};

}

MessageModel::MessageModel(Monitor *monitor, QObject *parent)
    : EntityTreeModel(monitor, parent)
{
    // Make sure we have an envelope
    monitor->itemFetchScope().fetchPayloadPart(MessagePart::Envelope);
    setCollectionFetchStrategy(InvisibleCollectionFetch);
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
    if (!item.hasPayload<KMime::Message::Ptr>()) {
        return {};
    }

    const auto msg = item.payload<KMime::Message::Ptr>();
    if (role == Qt::DisplayRole) {
        switch (column) {
        case Subject:
            if (msg->subject(false)) {
                return msg->subject(false)->asUnicodeString();
            } else {
                return i18nc("@label Alternative text when email subject is missing", "(No subject)");
            }
        case Sender:
            if (msg->from(false)) {
                return msg->from(false)->asUnicodeString();
            } else {
                return i18nc("@label Alternative text when email sender is missing", "(No sender)");
            }
        case Receiver:
            if (msg->to(false)) {
                return msg->to(false)->asUnicodeString();
            } else {
                return i18nc("@label Alternative text when email recipient is missing", "(No receiver)");
            }
        case Date:
            if (msg->date(false)) {
                return QLocale().toString(msg->date()->dateTime());
            } else {
                return i18nc("@label Alternative text when email date/time is missing", "(No date)");
            }
        case Size:
            if (item.size() == 0) {
                return i18nc("@label No size available", "-");
            } else {
                return KFormat().formatByteSize(item.size());
            }
        default:
            return QVariant();
        }
    } else if (role == Qt::EditRole) {
        switch (column) {
        case Subject:
            return msg->subject()->asUnicodeString();
        case Sender:
            return msg->from()->asUnicodeString();
        case Receiver:
            return msg->to()->asUnicodeString();
        case Date:
            return msg->date()->dateTime() /*.dateTime()*/;
        case Size:
            return item.size();
        default:
            return QVariant();
        }
    }

    return EntityTreeModel::entityData(item, column, role);
}

QVariant MessageModel::entityHeaderData(int section, Qt::Orientation orientation, int role, HeaderGroup headerGroup) const
{
    if (section >= 0 && static_cast<std::size_t>(section) < headers.size()) {
        return i18nc(headers[section].desc, headers[section].name);
    }

    return EntityTreeModel::entityHeaderData(section, orientation, role, headerGroup);
}

/*
    SPDX-FileCopyrightText: 2009 Kevin Ottens <ervin@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagefolderattribute.h"

using namespace Akonadi;

class Akonadi::MessageFolderAttributePrivate
{
public:
    bool isOutboundFolder = false;
};

MessageFolderAttribute::MessageFolderAttribute()
    : d(new MessageFolderAttributePrivate)
{
}

MessageFolderAttribute::MessageFolderAttribute(const MessageFolderAttribute &other)
    : Attribute(other)
    , d(new MessageFolderAttributePrivate(*(other.d)))
{
}

MessageFolderAttribute::~MessageFolderAttribute() = default;

QByteArray MessageFolderAttribute::type() const
{
    static const QByteArray sType("MESSAGEFOLDER");
    return sType;
}

MessageFolderAttribute *MessageFolderAttribute::clone() const
{
    return new MessageFolderAttribute(*this);
}

QByteArray MessageFolderAttribute::serialized() const
{
    QByteArray rv;

    if (d->isOutboundFolder) {
        rv = "outbound";
    } else {
        rv = "inbound";
    }

    return rv;
}

void MessageFolderAttribute::deserialize(const QByteArray &data)
{
    if (data == "outbound") {
        d->isOutboundFolder = true;
    } else {
        d->isOutboundFolder = false;
    }
}

bool MessageFolderAttribute::isOutboundFolder() const
{
    return d->isOutboundFolder;
}

void MessageFolderAttribute::setOutboundFolder(bool outbound)
{
    d->isOutboundFolder = outbound;
}

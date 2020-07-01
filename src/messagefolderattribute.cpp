/*
    SPDX-FileCopyrightText: 2009 Kevin Ottens <ervin@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagefolderattribute.h"

using namespace Akonadi;

class Akonadi::MessageFolderAttribute::Private
{
public:
    Private()
        : isOutboundFolder(false)
    {
    }

    bool isOutboundFolder;
};

MessageFolderAttribute::MessageFolderAttribute()
    : d(new Private)
{
}

MessageFolderAttribute::MessageFolderAttribute(const MessageFolderAttribute &other)
    : Attribute(other)
    , d(new Private(*(other.d)))
{
}

MessageFolderAttribute::~MessageFolderAttribute()
{
    delete d;
}

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

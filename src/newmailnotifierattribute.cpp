/*
    SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "newmailnotifierattribute.h"

#include <QByteArray>
#include <QDataStream>
#include <QIODevice>

namespace Akonadi
{
class NewMailNotifierAttributePrivate
{
public:
    bool ignoreNewMail = false;
};

NewMailNotifierAttribute::NewMailNotifierAttribute()
    : d(new NewMailNotifierAttributePrivate)
{
}

NewMailNotifierAttribute::~NewMailNotifierAttribute() = default;

NewMailNotifierAttribute *NewMailNotifierAttribute::clone() const
{
    auto attr = new NewMailNotifierAttribute();
    attr->setIgnoreNewMail(ignoreNewMail());
    return attr;
}

QByteArray NewMailNotifierAttribute::type() const
{
    static const QByteArray sType("newmailnotifierattribute");
    return sType;
}

QByteArray NewMailNotifierAttribute::serialized() const
{
    QByteArray result;
    QDataStream s(&result, QIODevice::WriteOnly);
    s << ignoreNewMail();
    return result;
}

void NewMailNotifierAttribute::deserialize(const QByteArray &data)
{
    QDataStream s(data);
    s >> d->ignoreNewMail;
}

bool NewMailNotifierAttribute::ignoreNewMail() const
{
    return d->ignoreNewMail;
}

void NewMailNotifierAttribute::setIgnoreNewMail(bool b)
{
    d->ignoreNewMail = b;
}

bool NewMailNotifierAttribute::operator==(const NewMailNotifierAttribute &other) const
{
    return d->ignoreNewMail == other.ignoreNewMail();
}
}

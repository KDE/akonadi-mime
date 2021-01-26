/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addressattribute.h"

#include <QDataStream>

#include <attributefactory.h>

using namespace Akonadi;

/**
  @internal
*/
class AddressAttribute::Private
{
public:
    QString mFrom;
    QStringList mTo;
    QStringList mCc;
    QStringList mBcc;
};

AddressAttribute::AddressAttribute(const QString &from, const QStringList &to, const QStringList &cc, const QStringList &bcc)
    : d(new Private)
{
    d->mFrom = from;
    d->mTo = to;
    d->mCc = cc;
    d->mBcc = bcc;
}

AddressAttribute::~AddressAttribute()
{
    delete d;
}

AddressAttribute *AddressAttribute::clone() const
{
    return new AddressAttribute(d->mFrom, d->mTo, d->mCc, d->mBcc);
}

QByteArray AddressAttribute::type() const
{
    static const QByteArray sType("AddressAttribute");
    return sType;
}

QByteArray AddressAttribute::serialized() const
{
    QByteArray serializedData;
    QDataStream serializer(&serializedData, QIODevice::WriteOnly);
    serializer.setVersion(QDataStream::Qt_4_5);
    serializer << d->mFrom;
    serializer << d->mTo;
    serializer << d->mCc;
    serializer << d->mBcc;
    return serializedData;
}

void AddressAttribute::deserialize(const QByteArray &data)
{
    QDataStream deserializer(data);
    deserializer.setVersion(QDataStream::Qt_4_5);
    deserializer >> d->mFrom;
    deserializer >> d->mTo;
    deserializer >> d->mCc;
    deserializer >> d->mBcc;
}

QString AddressAttribute::from() const
{
    return d->mFrom;
}

void AddressAttribute::setFrom(const QString &from)
{
    d->mFrom = from;
}

QStringList AddressAttribute::to() const
{
    return d->mTo;
}

void AddressAttribute::setTo(const QStringList &to)
{
    d->mTo = to;
}

QStringList AddressAttribute::cc() const
{
    return d->mCc;
}

void AddressAttribute::setCc(const QStringList &cc)
{
    d->mCc = cc;
}

QStringList AddressAttribute::bcc() const
{
    return d->mBcc;
}

void AddressAttribute::setBcc(const QStringList &bcc)
{
    d->mBcc = bcc;
}

// Register the attribute when the library is loaded.
namespace
{
bool address_dummy()
{
    using namespace Akonadi;
    AttributeFactory::registerAttribute<AddressAttribute>();
    return true;
}

const bool address_registered = address_dummy();
}

/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addressattribute.h"

#include <QDataStream>

#include <Akonadi/AttributeFactory>

using namespace Akonadi;

/**
  @internal
*/
class AddressAttribute::Private
{
public:
    bool mDSN = false;
    QString mFrom;
    QStringList mTo;
    QStringList mCc;
    QStringList mBcc;
};

AddressAttribute::AddressAttribute(const QString &from, const QStringList &to, const QStringList &cc, const QStringList &bcc, bool dsn)
    : d(new Private)
{
    d->mFrom = from;
    d->mTo = to;
    d->mCc = cc;
    d->mBcc = bcc;
    d->mDSN = dsn;
}

AddressAttribute::~AddressAttribute() = default;

AddressAttribute *AddressAttribute::clone() const
{
    return new AddressAttribute(d->mFrom, d->mTo, d->mCc, d->mBcc, d->mDSN);
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
    serializer << d->mDSN;
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
    if (!deserializer.atEnd()) {
        deserializer >> d->mDSN;
    }
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

bool AddressAttribute::deliveryStatusNotification() const
{
    return d->mDSN;
}

bool AddressAttribute::operator==(const AddressAttribute &other) const
{
    return d->mDSN == other.deliveryStatusNotification() && d->mBcc == other.bcc() && d->mTo == other.to() && d->mCc == other.cc() && d->mFrom == other.from();
}

void AddressAttribute::setDeliveryStatusNotification(bool b)
{
    d->mDSN = b;
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

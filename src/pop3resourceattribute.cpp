/*
    SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pop3resourceattribute.h"

#include <QByteArray>
#include <QIODevice>
#include <QDataStream>
namespace Akonadi
{

class Pop3ResourceAttributePrivate
{
public:
    QString accountName;
};

Pop3ResourceAttribute::Pop3ResourceAttribute()
    : d(new Pop3ResourceAttributePrivate)
{
}

Pop3ResourceAttribute::~Pop3ResourceAttribute() = default;

Pop3ResourceAttribute *Pop3ResourceAttribute::clone() const
{
    Pop3ResourceAttribute *attr = new Pop3ResourceAttribute();
    attr->setPop3AccountName(pop3AccountName());
    return attr;
}

QByteArray Pop3ResourceAttribute::type() const
{
    static const QByteArray sType("pop3resourceattribute");
    return sType;
}

QByteArray Pop3ResourceAttribute::serialized() const
{
    QByteArray result;
    QDataStream s(&result, QIODevice::WriteOnly);
    s << pop3AccountName();
    return result;
}

void Pop3ResourceAttribute::deserialize(const QByteArray &data)
{
    QDataStream s(data);
    QString value;
    s >> value;
    d->accountName = value;
}

QString Pop3ResourceAttribute::pop3AccountName() const
{
    return d->accountName;
}

void Pop3ResourceAttribute::setPop3AccountName(const QString &accountName)
{
    d->accountName = accountName;
}

bool Pop3ResourceAttribute::operator==(const Pop3ResourceAttribute &other) const
{
    return d->accountName == other.pop3AccountName();
}

} // namespace Akonadi

/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "errorattribute.h"

using namespace Akonadi;

class Akonadi::ErrorAttributePrivate
{
public:
    QString mMessage;
};

ErrorAttribute::ErrorAttribute(const QString &msg)
    : d(new ErrorAttributePrivate)
{
    d->mMessage = msg;
}

ErrorAttribute::~ErrorAttribute() = default;

ErrorAttribute *ErrorAttribute::clone() const
{
    return new ErrorAttribute(d->mMessage);
}

QByteArray ErrorAttribute::type() const
{
    static const QByteArray sType("ErrorAttribute");
    return sType;
}

QByteArray ErrorAttribute::serialized() const
{
    return d->mMessage.toUtf8();
}

void ErrorAttribute::deserialize(const QByteArray &data)
{
    d->mMessage = QString::fromUtf8(data);
}

QString ErrorAttribute::message() const
{
    return d->mMessage;
}

void ErrorAttribute::setMessage(const QString &msg)
{
    d->mMessage = msg;
}

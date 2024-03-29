/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
  SPDX-FileContributor: Tobias Koenig <tokoe@kdab.com>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sentactionattribute.h"

#include <QDataStream>
#include <QIODevice>
#include <QSharedData>

using namespace Akonadi;

class Akonadi::SentActionAttributeActionPrivate : public QSharedData
{
public:
    SentActionAttributeActionPrivate() = default;

    SentActionAttributeActionPrivate(const SentActionAttributeActionPrivate &other)

        = default;

    SentActionAttribute::Action::Type mType = SentActionAttribute::Action::Invalid;
    QVariant mValue;
};

SentActionAttribute::Action::Action()
    : d(new SentActionAttributeActionPrivate)
{
}

SentActionAttribute::Action::Action(Type type, const QVariant &value)
    : d(new SentActionAttributeActionPrivate)
{
    d->mType = type;
    d->mValue = value;
}

SentActionAttribute::Action::Action(const Action &other)

    = default;

SentActionAttribute::Action::~Action() = default;

SentActionAttribute::Action::Type SentActionAttribute::Action::type() const
{
    return d->mType;
}

QVariant SentActionAttribute::Action::value() const
{
    return d->mValue;
}

SentActionAttribute::Action &SentActionAttribute::Action::operator=(const Action &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

bool SentActionAttribute::Action::operator==(const Action &other) const
{
    return (d->mType == other.d->mType) && (d->mValue == other.d->mValue);
}

class Akonadi::SentActionAttributePrivate
{
public:
    SentActionAttribute::Action::List mActions;
};

SentActionAttribute::SentActionAttribute()
    : d(new SentActionAttributePrivate)
{
}

SentActionAttribute::~SentActionAttribute() = default;

void SentActionAttribute::addAction(Action::Type type, const QVariant &value)
{
    d->mActions.append(Action(type, value));
}

SentActionAttribute::Action::List SentActionAttribute::actions() const
{
    return d->mActions;
}

SentActionAttribute *SentActionAttribute::clone() const
{
    auto attribute = new SentActionAttribute;
    attribute->d->mActions = d->mActions;

    return attribute;
}

QByteArray SentActionAttribute::type() const
{
    static const QByteArray sType("SentActionAttribute");
    return sType;
}

QByteArray SentActionAttribute::serialized() const
{
    QVariantList list;
    list.reserve(d->mActions.count());
    for (const Action &action : std::as_const(d->mActions)) {
        QVariantMap map;
        map.insert(QString::number(action.type()), action.value());

        list << QVariant(map);
    }

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_6);
    stream << list;

    return data;
}

void SentActionAttribute::deserialize(const QByteArray &data)
{
    d->mActions.clear();

    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_4_6);

    QVariantList list;
    stream >> list;

    for (const QVariant &variant : std::as_const(list)) {
        const QVariantMap map = variant.toMap();
        QMap<QString, QVariant>::const_iterator it = map.cbegin();
        const QMap<QString, QVariant>::const_iterator itEnd = map.cend();
        for (; it != itEnd; ++it) {
            d->mActions << Action(static_cast<Action::Type>(it.key().toInt()), it.value());
        }
    }
}

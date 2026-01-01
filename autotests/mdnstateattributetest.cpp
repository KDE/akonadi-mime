/*
  SPDX-FileCopyrightText: 2014-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "mdnstateattributetest.h"
#include "mdnstateattribute.h"
#include <QTest>

MDNStateAttributeTest::MDNStateAttributeTest(QObject *parent)
    : QObject(parent)
{
}

MDNStateAttributeTest::~MDNStateAttributeTest() = default;

void MDNStateAttributeTest::shouldHaveDefaultValue()
{
    Akonadi::MDNStateAttribute attr;
    QCOMPARE(attr.mdnState(), Akonadi::MDNStateAttribute::MDNStateUnknown);
}

void MDNStateAttributeTest::shouldHaveType()
{
    Akonadi::MDNStateAttribute attr;
    QCOMPARE(attr.type(), QByteArray("MDNStateAttribute"));
}

void MDNStateAttributeTest::shouldSerializedAttribute()
{
    Akonadi::MDNStateAttribute attr;

    attr.setMDNState(Akonadi::MDNStateAttribute::MDNDenied);
    QCOMPARE(attr.mdnState(), Akonadi::MDNStateAttribute::MDNDenied);
    const QByteArray ba = attr.serialized();
    Akonadi::MDNStateAttribute result;
    result.deserialize(ba);
    QCOMPARE(attr, result);
}

void MDNStateAttributeTest::shouldCloneAttribute()
{
    Akonadi::MDNStateAttribute attr;
    attr.setMDNState(Akonadi::MDNStateAttribute::MDNDenied);

    Akonadi::MDNStateAttribute *result = attr.clone();
    QCOMPARE(attr, *result);
    delete result;
}

QTEST_MAIN(MDNStateAttributeTest)

#include "moc_mdnstateattributetest.cpp"

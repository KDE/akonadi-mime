/*
    SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "addressattributetest.h"
#include "addressattribute.h"
#include <QTest>
QTEST_MAIN(AddressAttributeTest)
AddressAttributeTest::AddressAttributeTest(QObject *parent)
    : QObject(parent)
{
}

void AddressAttributeTest::shouldHaveDefaultValues()
{
    Akonadi::AddressAttribute attr;
    QVERIFY(attr.from().isEmpty());
    QVERIFY(attr.to().isEmpty());
    QVERIFY(attr.cc().isEmpty());
    QVERIFY(attr.bcc().isEmpty());
    QVERIFY(!attr.deliveryStatusNotification());
}

void AddressAttributeTest::shouldDeserializeValues()
{
    Akonadi::AddressAttribute attr;
    attr.setFrom(QStringLiteral("from"));
    attr.setTo({QStringLiteral("to")});
    attr.setBcc({QStringLiteral("bcc")});
    attr.setCc({QStringLiteral("cc")});
    attr.setDeliveryStatusNotification(true);
    const QByteArray ba = attr.serialized();
    Akonadi::AddressAttribute result;
    result.deserialize(ba);
    QVERIFY(attr == result);
}

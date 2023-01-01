/*
  SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "newmailnotifierattributetest.h"
#include "newmailnotifierattribute.h"
#include <QTest>

using namespace Akonadi;

NewMailNotifierAttributeTest::NewMailNotifierAttributeTest(QObject *parent)
    : QObject(parent)
{
}

void NewMailNotifierAttributeTest::shouldHaveDefaultValue()
{
    NewMailNotifierAttribute attr;
    QVERIFY(!attr.ignoreNewMail());
}

void NewMailNotifierAttributeTest::shouldSetIgnoreNotification()
{
    NewMailNotifierAttribute attr;
    bool ignore = false;
    attr.setIgnoreNewMail(ignore);
    QCOMPARE(attr.ignoreNewMail(), ignore);
    ignore = true;
    attr.setIgnoreNewMail(ignore);
    QCOMPARE(attr.ignoreNewMail(), ignore);
}

void NewMailNotifierAttributeTest::shouldSerializedData()
{
    NewMailNotifierAttribute attr;
    attr.setIgnoreNewMail(true);
    QByteArray ba = attr.serialized();
    NewMailNotifierAttribute result;
    result.deserialize(ba);
    QVERIFY(attr == result);
}

void NewMailNotifierAttributeTest::shouldCloneAttribute()
{
    NewMailNotifierAttribute attr;
    attr.setIgnoreNewMail(true);
    NewMailNotifierAttribute *result = attr.clone();
    QVERIFY(attr == *result);
    delete result;
}

void NewMailNotifierAttributeTest::shouldHaveType()
{
    NewMailNotifierAttribute attr;
    QCOMPARE(attr.type(), QByteArray("newmailnotifierattribute"));
}

QTEST_MAIN(NewMailNotifierAttributeTest)

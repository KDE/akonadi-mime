/*
  SPDX-FileCopyrightText: 2014-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pop3resourceattributetest.h"
#include "pop3resourceattribute.h"
#include <QTest>

Pop3ResourceAttributeTest::Pop3ResourceAttributeTest(QObject *parent)
    : QObject(parent)
{
}

void Pop3ResourceAttributeTest::shouldHaveDefaultValue()
{
    Akonadi::Pop3ResourceAttribute attr;
    QVERIFY(attr.pop3AccountName().isEmpty());
}

void Pop3ResourceAttributeTest::shouldAssignValue()
{
    Akonadi::Pop3ResourceAttribute attr;
    QString accountName;
    attr.setPop3AccountName(accountName);
    QCOMPARE(attr.pop3AccountName(), accountName);
    accountName = QStringLiteral("foo");
    attr.setPop3AccountName(accountName);
    QCOMPARE(attr.pop3AccountName(), accountName);
    accountName.clear();
    attr.setPop3AccountName(accountName);
    QCOMPARE(attr.pop3AccountName(), accountName);
}

void Pop3ResourceAttributeTest::shouldDeserializeValue()
{
    Akonadi::Pop3ResourceAttribute attr;
    QString accountName = QStringLiteral("foo");
    attr.setPop3AccountName(accountName);
    const QByteArray ba = attr.serialized();
    Akonadi::Pop3ResourceAttribute result;
    result.deserialize(ba);
    QCOMPARE(attr, result);
}

void Pop3ResourceAttributeTest::shouldCloneAttribute()
{
    Akonadi::Pop3ResourceAttribute attr;
    QString accountName = QStringLiteral("foo");
    attr.setPop3AccountName(accountName);
    Akonadi::Pop3ResourceAttribute *result = attr.clone();
    QCOMPARE(attr, *result);
    delete result;
}

QTEST_MAIN(Pop3ResourceAttributeTest)

#include "moc_pop3resourceattributetest.cpp"

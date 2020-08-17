/*
  SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef NEWMAILNOTIFIERATTRIBUTETEST_H
#define NEWMAILNOTIFIERATTRIBUTETEST_H

#include <QObject>

class NewMailNotifierAttributeTest : public QObject
{
    Q_OBJECT
public:
    explicit NewMailNotifierAttributeTest(QObject *parent = nullptr);

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldSetIgnoreNotification();
    void shouldSerializedData();
    void shouldCloneAttribute();
    void shouldHaveType();
};

#endif // NEWMAILNOTIFIERATTRIBUTETEST_H

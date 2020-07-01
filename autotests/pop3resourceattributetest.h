/*
  SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef POP3RESOURCEATTRIBUTETEST_H
#define POP3RESOURCEATTRIBUTETEST_H

#include <QObject>

class Pop3ResourceAttributeTest : public QObject
{
    Q_OBJECT
public:
    explicit Pop3ResourceAttributeTest(QObject *parent = nullptr);

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
    void shouldDeserializeValue();
    void shouldCloneAttribute();
};

#endif // POP3RESOURCEATTRIBUTETEST_H

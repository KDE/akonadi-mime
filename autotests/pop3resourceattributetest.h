/*
  SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

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


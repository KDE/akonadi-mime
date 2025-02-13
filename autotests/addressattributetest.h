/*
    SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AddressAttributeTest : public QObject
{
    Q_OBJECT
public:
    explicit AddressAttributeTest(QObject *parent = nullptr);
    ~AddressAttributeTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldDeserializeValues();
};

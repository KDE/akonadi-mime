/*
    SPDX-FileCopyrightText: 2007 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MailSerializerTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testEnvelopeDeserialize_data();
    void testEnvelopeDeserialize();

    void testEnvelopeSerialize_data();
    void testEnvelopeSerialize();

    void testParts();
    void testHeaderFetch();
    void testMultiDeserialize();
};


/*
    SPDX-FileCopyrightText: 2007 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MAILSERIALIZERPLUGINTEST_H
#define MAILSERIALIZERPLUGINTEST_H

#include <QObject>

class MailSerializerPluginTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testMailPlugin();
    void testMessageIntegrity();
};

#endif

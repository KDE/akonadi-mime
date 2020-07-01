/*
    SPDX-FileCopyrightText: 2007 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MESSAGETEST_TEST_H
#define MESSAGETEST_TEST_H

#include <kmime/kmime_message.h>
#include <QObject>

class MessageTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testCopyFlags();
private:
    KMime::Message::Ptr readAndParseMail(const QString &mailFile) const;
};

#endif

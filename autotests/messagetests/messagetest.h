/*
    SPDX-FileCopyrightText: 2007 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <KMime/Message>
#include <QObject>

class MessageTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testCopyFlags();

private:
    KMime::Message::Ptr readAndParseMail(const QString &mailFile) const;
};

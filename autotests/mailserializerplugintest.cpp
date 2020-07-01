/*
    SPDX-FileCopyrightText: 2007 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "mailserializerplugintest.h"

#include <AkonadiCore/Item>
#include <KMime/Message>

#include <QDebug>
#include <QTest>

QTEST_GUILESS_MAIN(MailSerializerPluginTest)

using namespace Akonadi;
using namespace KMime;

void MailSerializerPluginTest::testMailPlugin()
{
    QByteArray serialized
        = "From: sender@test.org\n"
          "Subject: Serializer Test\n"
          "To: receiver@test.org\n"
          "Date: Fri, 22 Jun 2007 17:24:24 +0000\n"
          "MIME-Version: 1.0\n"
          "Content-Type: text/plain\n"
          "\n"
          "Body data.";

    // deserializing
    Item item;
    item.setMimeType(QStringLiteral("message/rfc822"));
    item.setPayloadFromData(serialized);

    QVERIFY(item.hasPayload<KMime::Message::Ptr>());
    KMime::Message::Ptr msg = item.payload<KMime::Message::Ptr>();
    QVERIFY(msg != nullptr);

    QCOMPARE(msg->to()->asUnicodeString(), QStringLiteral("receiver@test.org"));
    QCOMPARE(msg->body(), QByteArray("Body data."));

    // serializing
    QByteArray data = item.payloadData();
    QCOMPARE(data, serialized);
}

void MailSerializerPluginTest::testMessageIntegrity()
{
    // A message that will be slightly modified if KMime::Content::assemble() is
    // called.  We want to avoid this, because it breaks signatures.
    QByteArray serialized
        = "from: sender@example.com\n"
          "to: receiver@example.com\n"
          "Subject: Serializer Test\n"
          "Date: Thu, 30 Jul 2009 13:46:31 +0300\n"
          "MIME-Version: 1.0\n"
          "Content-type: text/plain; charset=us-ascii\n"
          "\n"
          "Bla bla bla.";

    // Deserialize.
    Item item;
    item.setMimeType(QStringLiteral("message/rfc822"));
    item.setPayloadFromData(serialized);

    QVERIFY(item.hasPayload<KMime::Message::Ptr>());
    KMime::Message::Ptr msg = item.payload<KMime::Message::Ptr>();
    QVERIFY(msg != nullptr);

    qDebug() << "original data:" << serialized;
    qDebug() << "message content:" << msg->encodedContent();
    QCOMPARE(msg->encodedContent(), serialized);

    // Serialize.
    QByteArray data = item.payloadData();
    qDebug() << "original data:" << serialized;
    qDebug() << "serialized data:" << data;
    QCOMPARE(data, serialized);
}

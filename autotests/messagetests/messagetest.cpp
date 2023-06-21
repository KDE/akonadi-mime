/*
    SPDX-FileCopyrightText: 2007 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetest.h"
#include <Akonadi/Item>
#include <QFile>
#include <QTest>
#include <messageflags.h>
using namespace KMime;

QTEST_GUILESS_MAIN(MessageTest)

void MessageTest::testCopyFlags()
{
    {
        KMime::Message::Ptr msg = readAndParseMail(QStringLiteral("x-pkcs7.mbox"));

        Akonadi::Item item;
        Akonadi::MessageFlags::copyMessageFlags(*msg, item);

        QVERIFY(item.hasFlag(Akonadi::MessageFlags::Signed) == false);
        QVERIFY(item.hasFlag(Akonadi::MessageFlags::Encrypted) == true);
        QVERIFY(item.hasFlag(Akonadi::MessageFlags::HasInvitation) == false);
        QVERIFY(item.hasFlag(Akonadi::MessageFlags::HasAttachment) == false);
    }

    {
        KMime::Message::Ptr msg = readAndParseMail(QStringLiteral("signed.mbox"));

        Akonadi::Item item;
        Akonadi::MessageFlags::copyMessageFlags(*msg, item);

        QVERIFY(item.hasFlag(Akonadi::MessageFlags::Signed) == true);
        QVERIFY(item.hasFlag(Akonadi::MessageFlags::Encrypted) == false);
        QVERIFY(item.hasFlag(Akonadi::MessageFlags::HasInvitation) == true);
        QVERIFY(item.hasFlag(Akonadi::MessageFlags::HasAttachment) == true);
    }
}

KMime::Message::Ptr MessageTest::readAndParseMail(const QString &mailFile) const
{
    QFile file(QLatin1String(TEST_DATA_DIR) + QLatin1String("/mails/") + mailFile);
    const bool ok = file.open(QIODevice::ReadOnly);
    if (!ok) {
        qWarning() << file.fileName() << "not found";
    }
    Q_ASSERT(ok);
    const QByteArray data = KMime::CRLFtoLF(file.readAll());
    Q_ASSERT(!data.isEmpty());
    KMime::Message::Ptr msg(new KMime::Message);
    msg->setContent(data);
    msg->parse();
    return msg;
}

#include "moc_messagetest.cpp"

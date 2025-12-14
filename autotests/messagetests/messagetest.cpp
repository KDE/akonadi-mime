/*
    SPDX-FileCopyrightText: 2007 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagetest.h"
#include "messageflags.h"
#include <Akonadi/Item>
#include <QFile>
#include <QTest>
using namespace KMime;

QTEST_GUILESS_MAIN(MessageTest)

void MessageTest::testCopyFlags()
{
    {
        std::shared_ptr<KMime::Message> msg = readAndParseMail(QStringLiteral("x-pkcs7.mbox"));

        Akonadi::Item item;
        Akonadi::MessageFlags::copyMessageFlags(*msg, item);

        QCOMPARE(item.hasFlag(Akonadi::MessageFlags::Signed), false);
        QCOMPARE(item.hasFlag(Akonadi::MessageFlags::Encrypted), true);
        QCOMPARE(item.hasFlag(Akonadi::MessageFlags::HasInvitation), false);
        QCOMPARE(item.hasFlag(Akonadi::MessageFlags::HasAttachment), false);
    }

    {
        std::shared_ptr<KMime::Message> msg = readAndParseMail(QStringLiteral("signed.mbox"));

        Akonadi::Item item;
        Akonadi::MessageFlags::copyMessageFlags(*msg, item);

        QCOMPARE(item.hasFlag(Akonadi::MessageFlags::Signed), true);
        QCOMPARE(item.hasFlag(Akonadi::MessageFlags::Encrypted), false);
        QCOMPARE(item.hasFlag(Akonadi::MessageFlags::HasInvitation), true);
        QCOMPARE(item.hasFlag(Akonadi::MessageFlags::HasAttachment), true);
    }
}

std::shared_ptr<KMime::Message> MessageTest::readAndParseMail(const QString &mailFile) const
{
    QFile file(QLatin1StringView(TEST_DATA_DIR) + QLatin1StringView("/mails/") + mailFile);
    const bool ok = file.open(QIODevice::ReadOnly);
    if (!ok) {
        qWarning() << file.fileName() << "not found";
    }
    Q_ASSERT(ok);
    const QByteArray data = KMime::CRLFtoLF(file.readAll());
    Q_ASSERT(!data.isEmpty());
    std::shared_ptr<KMime::Message> msg(new KMime::Message);
    msg->setContent(data);
    msg->parse();
    return msg;
}

#include "moc_messagetest.cpp"

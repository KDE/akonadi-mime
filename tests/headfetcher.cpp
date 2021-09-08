/*
    SPDX-FileCopyrightText: 2007 Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "headfetcher.h"

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/ItemFetchJob>
#include <akonadi/itemfetchscope.h>

#include "messageparts.h"

#include <QApplication>
#include <QDebug>
#include <QTimer>

#include <kmime/kmime_message.h>

#include <KAboutData>
#include <KLocalizedString>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <chrono>

using namespace std::chrono_literals;

using namespace Akonadi;

HeadFetcher::HeadFetcher(bool multipart)
{
    // fetch all headers from each folder
    timer.start();
    qDebug() << "Listing all headers of every folder, using" << (multipart ? "multi" : "single") << "part.";
    auto clj = new CollectionFetchJob(Collection::root(), CollectionFetchJob::Recursive);
    clj->exec();
    const Collection::List list = clj->collections();
    for (const Collection &collection : list) {
        auto ifj = new ItemFetchJob(collection, this);
        if (multipart) {
            ifj->fetchScope().fetchPayloadPart(MessagePart::Envelope);
        } else {
            ifj->fetchScope().fetchFullPayload();
        }
        ifj->exec();
        qDebug() << "  Listing" << ifj->items().count() << "item headers.";
        const auto items = ifj->items();
        for (const Item &item : items) {
            qDebug() << item.payload<KMime::Message::Ptr>()->subject()->asUnicodeString();
        }
    }

    qDebug() << "Took:" << timer.elapsed() << "ms.";
    QTimer::singleShot(1s, this, &HeadFetcher::stop);
}

void HeadFetcher::stop()
{
    qApp->quit();
}

int main(int argc, char *argv[])
{
    KAboutData aboutData(QStringLiteral("headfetcher"), i18n("Headfetcher"), QStringLiteral("1.0"));
    aboutData.setShortDescription(i18n("header fetching application"));
    QApplication app(argc, argv);
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("multipart"), i18n("Run test on multipart data (default is singlepart).")));

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    bool multipart = parser.isSet(QStringLiteral("multipart"));

    HeadFetcher d(multipart);

    return app.exec();
}

/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "maildirfetchunreadheaders.h"

#include <QDebug>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/ItemFetchJob>
#include <akonadi/itemfetchscope.h>

#include "akonadi/kmime/messageparts.h"
#include <kmime/kmime_message.h>

using namespace Akonadi;

MailDirFetchUnreadHeaders::MailDirFetchUnreadHeaders()
    : MailDir()
{
}

void MailDirFetchUnreadHeaders::runTest()
{
    timer.start();
    qDebug() << "  Listing headers of unread messages of every folder.";
    auto clj3 = new CollectionFetchJob(Collection::root(), CollectionFetchJob::Recursive);
    clj3->fetchScope().setResource(currentInstance.identifier());
    clj3->exec();
    const Collection::List list3 = clj3->collections();
    for (const Collection &collection : list3) {
        auto ifj = new ItemFetchJob(collection, this);
        ifj->fetchScope().fetchPayloadPart(MessagePart::Envelope);
        ifj->exec();
        QString a;
        const auto items = ifj->items();
        for (const Item &item : items) {
            // filter read messages
            if (!item.hasFlag("\\SEEN")) {
                a = item.payload<KMime::Message::Ptr>()->subject()->asUnicodeString();
            }
        }
    }
    outputStats(QStringLiteral("unreadheaderlist"));
}

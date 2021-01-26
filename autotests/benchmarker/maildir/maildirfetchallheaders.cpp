/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "maildirfetchallheaders.h"

#include <QDebug>

#include <collectionfetchjob.h>
#include <collectionfetchscope.h>
#include <itemfetchjob.h>
#include <itemfetchscope.h>

#include "akonadi/kmime/messageparts.h"
#include <kmime/kmime_message.h>

using namespace Akonadi;

MailDirFetchAllHeaders::MailDirFetchAllHeaders()
    : MailDir()
{
}

void MailDirFetchAllHeaders::runTest()
{
    timer.start();
    qDebug() << "  Listing all headers of every folder.";
    auto clj = new CollectionFetchJob(Collection::root(), CollectionFetchJob::Recursive);
    clj->fetchScope().setResource(currentInstance.identifier());
    clj->exec();
    const Collection::List list = clj->collections();
    for (const Collection &collection : list) {
        auto ifj = new ItemFetchJob(collection, this);
        ifj->fetchScope().fetchPayloadPart(MessagePart::Envelope);
        ifj->exec();
        QString a;
        const auto items = ifj->items();
        for (const Item &item : items) {
            a = item.payload<KMime::Message::Ptr>()->subject()->asUnicodeString();
        }
    }
    outputStats(QStringLiteral("fullheaderlist"));
}

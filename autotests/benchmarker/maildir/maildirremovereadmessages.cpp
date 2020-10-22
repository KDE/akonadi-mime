/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "maildirremovereadmessages.h"
#include <QDebug>

#include <collectionfetchjob.h>
#include <collectionfetchscope.h>
#include <itemdeletejob.h>
#include <itemfetchjob.h>

using namespace Akonadi;

MailDirRemoveReadMessages::MailDirRemoveReadMessages()
    : MailDir()
{
}

void MailDirRemoveReadMessages::runTest()
{
    timer.start();
    qDebug() << "  Removing read messages from every folder.";
    CollectionFetchJob *clj4 = new CollectionFetchJob(Collection::root(), CollectionFetchJob::Recursive);
    clj4->fetchScope().setResource(currentInstance.identifier());
    clj4->exec();
    const Collection::List list4 = clj4->collections();
    for (const Collection &collection : list4) {
        ItemFetchJob *ifj = new ItemFetchJob(collection, this);
        ifj->exec();
        const auto items = ifj->items();
        for (const Item &item : items) {
            // delete read messages
            if (item.hasFlag("\\SEEN")) {
                ItemDeleteJob *idj = new ItemDeleteJob(item, this);
                idj->exec();
            }
        }
    }
    outputStats(QStringLiteral("removereaditems"));
}

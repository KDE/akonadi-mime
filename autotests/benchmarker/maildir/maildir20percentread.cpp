/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "maildir20percentread.h"

#include <QDebug>

#include <collectionfetchjob.h>
#include <collectionfetchscope.h>
#include <itemfetchjob.h>
#include <itemmodifyjob.h>

using namespace Akonadi;

MailDir20PercentAsRead::MailDir20PercentAsRead()
    : MailDir()
{
}

void MailDir20PercentAsRead::runTest()
{
    timer.start();
    qDebug() << "  Marking 20% of messages as read.";
    auto clj2 = new CollectionFetchJob(Collection::root(), CollectionFetchJob::Recursive);
    clj2->fetchScope().setResource(currentInstance.identifier());
    clj2->exec();
    const Collection::List list2 = clj2->collections();
    for (const Collection &collection : list2) {
        auto ifj = new ItemFetchJob(collection, this);
        ifj->exec();
        Item::List itemlist = ifj->items();
        for (int i = ifj->items().count() - 1; i >= 0; i -= 5) {
            Item item = itemlist[i];
            item.setFlag("\\SEEN");
            auto isj = new ItemModifyJob(item, this);
            isj->exec();
        }
    }
    outputStats(QStringLiteral("mark20percentread"));
}

/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "maildir.h"

#include <Akonadi/CollectionDeleteJob>
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/ItemDeleteJob>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/ItemModifyJob>

#include "akonadi/messageparts.h"
#include <KMime/Message>

#define WAIT_TIME 100

using namespace Akonadi;

MailDir::MailDir(const QString &dir)
    : MakeTest()
{
    createAgent(QStringLiteral("akonadi_maildir_resource"));
    configureDBusIface(QStringLiteral("Maildir"), dir);
}

MailDir::MailDir()
    : MakeTest()
{
}

#include "moc_maildir.cpp"

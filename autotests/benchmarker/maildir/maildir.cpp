/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "maildir.h"

#include <collectiondeletejob.h>
#include <collectionfetchjob.h>
#include <itemdeletejob.h>
#include <itemfetchjob.h>
#include <itemfetchscope.h>
#include <itemmodifyjob.h>

#include "akonadi/kmime/messageparts.h"
#include <kmime/kmime_message.h>

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

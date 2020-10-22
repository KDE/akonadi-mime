/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "maildirimport.h"
#include <QDebug>
#include <QTest>

#define WAIT_TIME 100

MailDirImport::MailDirImport(const QString &dir)
    : MailDir(dir)
{
}

void MailDirImport::runTest()
{
    done = false;
    timer.start();
    qDebug() << "  Synchronising resource.";
    currentInstance.synchronize();
    while (!done) {
        QTest::qWait(WAIT_TIME);
    }
    outputStats(QStringLiteral("import"));
}

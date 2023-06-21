/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vcardimport.h"
#include <QDebug>
#include <QTest>

#define WAIT_TIME 50

VCardImport::VCardImport(const QString &dir)
    : VCard(dir)
{
}

void VCardImport::runTest()
{
    done = false;
    timer.start();
    qDebug() << "Synchronising resource";
    currentInstance.synchronize();
    while (!done) {
        QTest::qWait(WAIT_TIME);
    }
    outputStats(QStringLiteral("import"));
}

#include "moc_vcardimport.cpp"

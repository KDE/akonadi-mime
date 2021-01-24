/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "testmaildir.h"
#include "testvcard.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("maildir"), QStringLiteral("Path to maildir to be used as data source"), QStringLiteral("argument")));
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("vcarddir"), QStringLiteral("Path to vvcarddir to be used as data source"), QStringLiteral("argument")));

    parser.process(app);

    const QString maildir = parser.value(QStringLiteral("maildir"));
    const QString vcarddir = parser.value(QStringLiteral("vcarddir"));

    auto mailDirTest = new TestMailDir(maildir);
    auto vcardTest = new TestVCard(vcarddir);

    mailDirTest->runTests();
    vcardTest->runTests();

    return app.exec();
}

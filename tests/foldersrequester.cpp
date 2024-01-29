/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "foldersrequester.h"

#include "specialmailcollections.h"
#include "specialmailcollectionsrequestjob.h"
#include <Akonadi/Collection>
#include <Akonadi/Control>
#include <KAboutData>
#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

using namespace Akonadi;

Requester::Requester()
{
    Control::start();

    auto rjob = new SpecialMailCollectionsRequestJob(this);
    rjob->requestDefaultCollection(SpecialMailCollections::Outbox);
    connect(rjob, &SpecialMailCollectionsRequestJob::result, this, &Requester::requestResult);
    rjob->start();
}

void Requester::requestResult(KJob *job)
{
    if (job->error()) {
        qCritical() << "LocalFoldersRequestJob failed:" << job->errorString();
        QApplication::exit(1);
    } else {
        // Success.
        QApplication::exit(2);
    }
}

int main(int argc, char **argv)
{
    QCommandLineParser parser;

    KAboutData about(QStringLiteral("foldersrequester"), i18n("An app that requests LocalFolders"), QStringLiteral("0.1"));

    about.setupCommandLine(&parser);
    KAboutData::setApplicationData(about);

    QApplication app(argc, argv);
    parser.process(app);
    about.processCommandLine(&parser);

    new Requester();
    return app.exec();
}

#include "moc_foldersrequester.cpp"

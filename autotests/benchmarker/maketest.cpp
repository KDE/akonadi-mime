/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "test.h"

#include <Akonadi/AgentInstanceCreateJob>
#include <Akonadi/CollectionDeleteJob>
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>
#include <QTest>

using namespace Akonadi;

MakeTest::MakeTest()
{
    connect(AgentManager::self(), &AgentManager::instanceRemoved, this, &MakeTest::instanceRemoved);
    connect(AgentManager::self(), &AgentManager::instanceStatusChanged, this, &MakeTest::instanceStatusChanged);
}

void MakeTest::createAgent(const QString &name)
{
    const AgentType type = AgentManager::self()->type(name);

    auto job = new AgentInstanceCreateJob(type);
    job->exec();
    currentInstance = job->instance();

    if (job->error() || !currentInstance.isValid()) {
        qDebug() << "  Unable to create resource" << name;
        exit(-1);
    } else {
        qDebug() << "  Created resource instance" << currentInstance.identifier();
    }

    QTest::qWait(100); // fix this hack
}

void MakeTest::configureDBusIface(const QString &name, const QString &dir)
{
    auto configIface = new QDBusInterface(QLatin1String("org.freedesktop.Akonadi.Resource.") + currentInstance.identifier(),
                                          QStringLiteral("/Settings"),
                                          QLatin1String("org.kde.Akonadi.") + name + QLatin1String(".Settings"),
                                          QDBusConnection::sessionBus(),
                                          this);

    configIface->call(QStringLiteral("setPath"), dir);
    configIface->call(QStringLiteral("setReadOnly"), true);

    if (!configIface->isValid()) {
        qFatal("Could not configure instance %s.", qPrintable(currentInstance.identifier()));
    }
}

void MakeTest::instanceRemoved(const AgentInstance &instance)
{
    Q_UNUSED(instance)
    done = true;
    // qDebug() << "agent removed:" << instance;
}

void MakeTest::instanceStatusChanged(const AgentInstance &instance)
{
    // qDebug() << "agent status changed:" << agentIdentifier << status << message ;
    if (instance == currentInstance) {
        if (instance.status() == AgentInstance::Running) {
            // qDebug() << "    " << message;
        }
        if (instance.status() == AgentInstance::Idle) {
            done = true;
        }
    }
}

void MakeTest::outputStats(const QString &description)
{
    output(description + QLatin1String("\t\t") + currentAccount + QLatin1String("\t\t") + QString::number(timer.elapsed()) + QLatin1Char('\n'));
}

void MakeTest::output(const QString &message)
{
    QTextStream out(stdout);
    out << message;
}

void MakeTest::removeCollections()
{
    timer.restart();
    qDebug() << "  Removing every folder sequentially.";
    auto clj5 = new CollectionFetchJob(Collection::root(), CollectionFetchJob::Recursive);
    clj5->fetchScope().setResource(currentInstance.identifier());
    clj5->exec();
    const Collection::List list5 = clj5->collections();
    for (const Collection &collection : list5) {
        auto cdj = new CollectionDeleteJob(collection, this);
        cdj->exec();
    }
    outputStats(QStringLiteral("removeallcollections"));
}

void MakeTest::removeResource()
{
    qDebug() << "  Removing resource.";
    AgentManager::self()->removeInstance(currentInstance);
}

void MakeTest::start()
{
    runTest();
}

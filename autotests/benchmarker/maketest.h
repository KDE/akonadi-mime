/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <agentmanager.h>
#include <job.h>

#include <QElapsedTimer>

class MakeTest : public QObject
{
    Q_OBJECT
protected Q_SLOTS:
    void createAgent(const QString &name);
    void configureDBusIface(const QString &name, const QString &dir);
    void instanceRemoved(const Akonadi::AgentInstance &instance);
    void instanceStatusChanged(const Akonadi::AgentInstance &instance);
    void outputStats(const QString &description);
    void output(const QString &message);

protected:
    Akonadi::AgentInstance currentInstance;
    QString currentAccount;
    QElapsedTimer timer;
    bool done;
    void removeCollections();
    void removeResource();
    virtual void runTest() = 0;

public:
    MakeTest();
    void start();
};


/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QList>
#include <QObject>

class KProcess;

/**
  This tests the ability of LocalFolders to exist peacefully in multiple processes.
  The main instance (normally the first one created) is supposed to create the
  resource and collections, while the other instances are supposed to wait and
  then just fetch the collections.
 */
class RaceTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testMultipleProcesses_data();
    void testMultipleProcesses();
    void killZombies();

private:
    QList<KProcess *> procs;
};

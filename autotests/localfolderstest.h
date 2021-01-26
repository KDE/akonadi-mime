/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef LOCALFOLDERSTEST_H
#define LOCALFOLDERSTEST_H

#include <QMap>
#include <QObject>

/**
 */
namespace Akonadi
{
class LocalFoldersTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();

    // Tests for GetLockJob:
    void testLock();

    // Tests for LocalFolders:
    void testInitialState();
    void testRegistrationErrors();
    void testDefaultFolderRegistration();
    void testCustomFolderRegistration();
    void testCollectionDelete();
    void testBatchRegister();

    // Tests for ResourceScanJob:
    void testResourceScanErrors();
    void testResourceScan();

    // Tests for DefaultResourceJob:
    void testDefaultResourceJob();
    void testRecoverDefaultResource();

private:
    QMap<QByteArray, QString> mDisplayNameMap;
    QMap<QByteArray, QString> mIconNameMap;
};
}

#endif

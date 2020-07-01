/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef LOCALFOLDERSREQUESTJOBTEST_H
#define LOCALFOLDERSREQUESTJOBTEST_H

#include <QObject>

/**
 */
class LocalFoldersRequestJobTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testRequestWithNoDefaultResourceExisting();
    void testRequestWithDefaultResourceAlreadyExisting();
    void testMixedRequest();
};

#endif

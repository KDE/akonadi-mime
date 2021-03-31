/*
    SPDX-FileCopyrightText: 2007 Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QElapsedTimer>
#include <QObject>

class HeadFetcher : public QObject
{
    Q_OBJECT
public:
    HeadFetcher(bool multipart);
private Q_SLOTS:
    void stop();

private:
    QElapsedTimer timer;
};


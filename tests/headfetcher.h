/*
    SPDX-FileCopyrightText: 2007 Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef HEADFETCHER_H
#define HEADFETCHER_H

#include <QObject>
#include <QElapsedTimer>

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

#endif

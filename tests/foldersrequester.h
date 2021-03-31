/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class KJob;

/**
  This class requests some LocalFolders, then exits the app with a status of 2
  if they were delivered OK, or 1 if they were not.

  NOTE: The non-standard exit status 2 in case of success is to make feel more
  comfortable than checking for zero (I actually had a bug causing it to always
  return zero).
*/
class Requester : public QObject
{
    Q_OBJECT

public:
    Requester();

private Q_SLOTS:
    void requestResult(KJob *job);
};


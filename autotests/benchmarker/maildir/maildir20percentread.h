/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MAILDIR20PERCENTREAD_H
#define MAILDIR20PERCENTREAD_H

#include "maildir.h"

class MailDir20PercentAsRead : public MailDir
{
    Q_OBJECT
public:
    MailDir20PercentAsRead();
    void runTest() override;
};
#endif

/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef MAILDIRREMOVEREADMESSAGES_H
#define MAILDIRREMOVEREADMESSAGES_H

#include "maildir.h"

class MailDirRemoveReadMessages : public MailDir
{
    Q_OBJECT
public:
    MailDirRemoveReadMessages();
    void runTest() override;
};
#endif

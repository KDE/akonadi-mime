/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "../maketest.h"

class MailDir : public MakeTest
{
    Q_OBJECT
public:
    MailDir(const QString &dir);
    MailDir();
};

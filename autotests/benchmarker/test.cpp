/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>
    based on kdepimlibs/akonadi/tests/benchmarker.cpp wrote by Robert Zwerus <arzie@dds.nl>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "test.h"

void Test::addTest(MakeTest *test)
{
    mListTest.append(test);
}

void Test::runTests()
{
    for (int i = 0; i < mListTest.size(); ++i) {
        mListTest.at(i)->start();
    }
}

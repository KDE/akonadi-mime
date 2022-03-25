/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "../maketest.h"

class VCard : public MakeTest
{
    Q_OBJECT
public:
    VCard(const QString &dir);
    VCard();
};

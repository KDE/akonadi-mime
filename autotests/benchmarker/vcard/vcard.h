/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef VCARD_H
#define VCARD_H

#include "../maketest.h"

class VCard : public MakeTest
{
    Q_OBJECT
public:
    VCard(const QString &dir);
    VCard();
};

#endif

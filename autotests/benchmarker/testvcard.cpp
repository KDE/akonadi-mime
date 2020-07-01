/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "testvcard.h"
#include "vcard/vcardimport.h"

TestVCard::TestVCard(const QString &dir)
{
    addTest(new VCardImport(dir));
}

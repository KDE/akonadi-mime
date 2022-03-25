/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "vcard.h"

class VCardImport : public VCard
{
    Q_OBJECT
public:
    VCardImport(const QString &dir);
    void runTest() override;
};

/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "vcard.h"

VCard::VCard(const QString &dir)
    : MakeTest()
{
    createAgent(QStringLiteral("akonadi_vcarddir_resource"));
    configureDBusIface(QStringLiteral("VCard"), dir);
}

VCard::VCard()
    : MakeTest()
{
}

#include "moc_vcard.cpp"

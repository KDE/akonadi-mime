/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagefolderattribute.h"

#include <attributefactory.h>

#ifndef KDELIBS_STATIC_LIBS
namespace
{
// Anonymous namespace; function is invisible outside this file.
bool messagefolder_dummy()
{
    Akonadi::AttributeFactory::registerAttribute<Akonadi::MessageFolderAttribute>();
    return true;
}

// Called when this library is loaded.
const bool registered = messagefolder_dummy();
} // namespace

#else

extern bool ___AkonadiKMime____INIT()
{
    Akonadi::AttributeFactory::registerAttribute<Akonadi::MessageFolderAttribute>();
    return true;
}

#endif

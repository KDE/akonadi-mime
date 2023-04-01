/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagefolderattribute.h"

#include "dispatchmodeattribute.h"
#include "errorattribute.h"
#include "sentactionattribute.h"
#include "sentbehaviourattribute.h"
#include "transportattribute.h"

#include <Akonadi/AttributeFactory>

#ifndef KDELIBS_STATIC_LIBS
namespace
{
// Anonymous namespace; function is invisible outside this file.
bool messagefolder_dummy()
{
    Akonadi::AttributeFactory::registerAttribute<Akonadi::MessageFolderAttribute>();
    Akonadi::AttributeFactory::registerAttribute<Akonadi::DispatchModeAttribute>();
    Akonadi::AttributeFactory::registerAttribute<Akonadi::ErrorAttribute>();
    Akonadi::AttributeFactory::registerAttribute<Akonadi::SentActionAttribute>();
    Akonadi::AttributeFactory::registerAttribute<Akonadi::SentBehaviourAttribute>();
    Akonadi::AttributeFactory::registerAttribute<Akonadi::TransportAttribute>();
    return true;
}

// Called when this library is loaded.
const bool registered = messagefolder_dummy();
} // namespace

#else

extern bool ___AkonadiKMime____INIT()
{
    Akonadi::AttributeFactory::registerAttribute<Akonadi::MessageFolderAttribute>();
    Akonadi::AttributeFactory::registerAttribute<Akonadi::DispatchModeAttribute>();
    Akonadi::AttributeFactory::registerAttribute<Akonadi::ErrorAttribute>();
    Akonadi::AttributeFactory::registerAttribute<Akonadi::SentActionAttribute>();
    Akonadi::AttributeFactory::registerAttribute<Akonadi::SentBehaviourAttribute>();
    Akonadi::AttributeFactory::registerAttribute<Akonadi::TransportAttribute>();
    return true;
}

#endif

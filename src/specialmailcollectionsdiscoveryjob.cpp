/*
    SPDX-FileCopyrightText: 2013 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "specialmailcollectionsdiscoveryjob.h"
#include "specialmailcollections.h"

#include <KMime/KMimeMessage>

#include <QStringList>

using namespace Akonadi;

SpecialMailCollectionsDiscoveryJob::SpecialMailCollectionsDiscoveryJob(QObject *parent)
    : SpecialCollectionsDiscoveryJob(SpecialMailCollections::self(), QStringList() << KMime::Message::mimeType(), parent)
{
}

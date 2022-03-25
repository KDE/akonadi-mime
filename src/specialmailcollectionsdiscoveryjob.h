/*
    SPDX-FileCopyrightText: 2013 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"

#include <Akonadi/SpecialCollectionsDiscoveryJob>

namespace Akonadi
{
class AKONADI_MIME_EXPORT SpecialMailCollectionsDiscoveryJob : public SpecialCollectionsDiscoveryJob
{
    Q_OBJECT
public:
    explicit SpecialMailCollectionsDiscoveryJob(QObject *parent = nullptr);
};
}

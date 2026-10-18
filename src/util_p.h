/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#pragma once

#include <QString>

class OrgKdeAkonadiImapSettingsInterface;
class KJob;
#define IMAP_RESOURCE_IDENTIFIER QStringLiteral("akonadi_imap_resource")

namespace Util
{
/// Helper to sanely show an error message for a job
void showJobError(KJob *job);

OrgKdeAkonadiImapSettingsInterface *createImapSettingsInterface(const QString &ident);
}

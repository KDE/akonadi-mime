/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "imapsettings.h"
#include "util_p.h"

#include <Akonadi/ServerManager>
#include <QDBusConnection>

#include "akonadi_mime_debug.h"
#include <KIO/Job>
#include <KJobUiDelegate>
#include <cassert>

namespace Util
{
/// Helper to sanely show an error message for a job
void showJobError(KJob *job)
{
    assert(job);
    // we can be called from the KJob::kill, where we are no longer a KIO::Job
    // so better safe than sorry
    auto kiojob = qobject_cast<KIO::Job *>(job);
    if (kiojob && kiojob->uiDelegate()) {
        kiojob->uiDelegate()->showErrorMessage();
    } else {
        qCWarning(AKONADIMIME_LOG) << "Job: " << job << "There is no GUI delegate set for a kjob, and it failed with error:" << job->errorString();
    }
}

OrgKdeAkonadiImapSettingsInterface *createImapSettingsInterface(const QString &ident)
{
    // NOTE(Andras): from kmail/util.cpp
    return new OrgKdeAkonadiImapSettingsInterface(Akonadi::ServerManager::agentServiceName(Akonadi::ServerManager::Resource, ident),
                                                  QStringLiteral("/Settings"),
                                                  QDBusConnection::sessionBus());
}
}

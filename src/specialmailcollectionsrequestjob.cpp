/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "specialmailcollectionsrequestjob.h"

#include <KLocalizedString>
#include <QStandardPaths>

using namespace Akonadi;

class Akonadi::SpecialMailCollectionsRequestJobPrivate
{
};

static inline QByteArray enumToType(SpecialMailCollections::Type type)
{
    switch (type) {
    case SpecialMailCollections::Root:
        return "local-mail";
    case SpecialMailCollections::Inbox:
        return "inbox";
    case SpecialMailCollections::Outbox:
        return "outbox";
    case SpecialMailCollections::SentMail:
        return "sent-mail";
    case SpecialMailCollections::Trash:
        return "trash";
    case SpecialMailCollections::Drafts:
        return "drafts";
    case SpecialMailCollections::Templates:
        return "templates";
    case SpecialMailCollections::LastType: // fallthrough
    default:
        return {};
    }
}

SpecialMailCollectionsRequestJob::SpecialMailCollectionsRequestJob(QObject *parent)
    : SpecialCollectionsRequestJob(SpecialMailCollections::self(), parent)
    , d(nullptr)
{
    static QMap<QByteArray, QString> displayNameMap;
    displayNameMap.insert("local-mail", i18nc("local mail folder", "Local Folders"));
    displayNameMap.insert("inbox", /*i18nc( "local mail folder",*/ QStringLiteral("inbox"));
    displayNameMap.insert("outbox", /*i18nc( "local mail folder",*/ QStringLiteral("outbox"));
    displayNameMap.insert("sent-mail", /*i18nc( "local mail folder",*/ QStringLiteral("sent-mail"));
    displayNameMap.insert("trash", /*i18nc( "local mail folder", */ QStringLiteral("trash"));
    displayNameMap.insert("drafts", /*i18nc( "local mail folder", */ QStringLiteral("drafts"));
    displayNameMap.insert("templates", /*i18nc( "local mail folder", */ QStringLiteral("templates"));

    static QMap<QByteArray, QString> iconNameMap;
    iconNameMap.insert("local-mail", QStringLiteral("folder"));
    iconNameMap.insert("inbox", QStringLiteral("mail-folder-inbox"));
    iconNameMap.insert("outbox", QStringLiteral("mail-folder-outbox"));
    iconNameMap.insert("sent-mail", QStringLiteral("mail-folder-sent"));
    iconNameMap.insert("trash", QStringLiteral("user-trash"));
    iconNameMap.insert("drafts", QStringLiteral("document-properties"));
    iconNameMap.insert("templates", QStringLiteral("document-new"));

    QVariantMap options;
    options.insert(QStringLiteral("Name"), displayNameMap.value("local-mail"));
    options.insert(QStringLiteral("TopLevelIsContainer"), true);
    options.insert(QStringLiteral("Path"),
                   QString(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + QLatin1String("local-mail")));

    setDefaultResourceType(QStringLiteral("akonadi_maildir_resource"));
    setDefaultResourceOptions(options);

    setTypes(displayNameMap.keys());
    setNameForTypeMap(displayNameMap);
    setIconForTypeMap(iconNameMap);
}

SpecialMailCollectionsRequestJob::~SpecialMailCollectionsRequestJob() = default;

void SpecialMailCollectionsRequestJob::requestDefaultCollection(SpecialMailCollections::Type type)
{
    SpecialCollectionsRequestJob::requestDefaultCollection(enumToType(type));
}

void SpecialMailCollectionsRequestJob::requestCollection(SpecialMailCollections::Type type, const AgentInstance &instance)
{
    SpecialCollectionsRequestJob::requestCollection(enumToType(type), instance);
}

#include "moc_specialmailcollectionsrequestjob.cpp"

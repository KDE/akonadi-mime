/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "specialmailcollections.h"
#include "akonadi_mime_debug.h"
#include "specialmailcollectionssettings.h"

#include <agentinstance.h>
#include <collectionmodifyjob.h>
#include <entitydisplayattribute.h>
#include <servermanager.h>
#include <specialcollectionattribute.h>

#include <KLocalizedString>
#include <kcoreaddons_version.h>
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <Kdelibs4ConfigMigrator>
#endif

using namespace Akonadi;

class Akonadi::SpecialMailCollectionsPrivate
{
public:
    SpecialMailCollectionsPrivate();
    ~SpecialMailCollectionsPrivate();

    SpecialMailCollections *mInstance = nullptr;
};

using Settings = SpecialMailCollectionsSettings;

Q_GLOBAL_STATIC(SpecialMailCollectionsPrivate, sInstance)

static const char s_specialCollectionTypes[SpecialMailCollections::LastType][11] =
    {"local-mail", "inbox", "outbox", "sent-mail", "trash", "drafts", "templates"};

static const int s_numTypes = sizeof s_specialCollectionTypes / sizeof *s_specialCollectionTypes;

static inline QByteArray enumToByteArray(SpecialMailCollections::Type value)
{
    return s_specialCollectionTypes[value];
}

static inline SpecialMailCollections::Type typeToEnum(const QByteArray &type)
{
    for (int i = 0; i < s_numTypes; ++i) {
        if (type == s_specialCollectionTypes[i]) {
            return static_cast<SpecialMailCollections::Type>(i);
        }
    }
    return SpecialMailCollections::Invalid;
}

SpecialMailCollectionsPrivate::SpecialMailCollectionsPrivate()
    : mInstance(new SpecialMailCollections(this))
{
}

SpecialMailCollectionsPrivate::~SpecialMailCollectionsPrivate()
{
    delete mInstance;
}

static KCoreConfigSkeleton *getConfig(const QString &filename)
{
#if KCOREADDONS_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Kdelibs4ConfigMigrator migrate(QStringLiteral("specialmailcollection"));
    migrate.setConfigFiles(QStringList() << filename);
    migrate.migrate();
#endif
    Settings::instance(ServerManager::addNamespace(filename));
    return Settings::self();
}

SpecialMailCollections::SpecialMailCollections(SpecialMailCollectionsPrivate *dd)
    : SpecialCollections(getConfig(QStringLiteral("specialmailcollectionsrc")))
    , d(dd)
{
}

SpecialMailCollections *SpecialMailCollections::self()
{
    return sInstance->mInstance;
}

bool SpecialMailCollections::hasCollection(Type type, const AgentInstance &instance) const
{
    return SpecialCollections::hasCollection(enumToByteArray(type), instance);
}

Collection SpecialMailCollections::collection(Type type, const AgentInstance &instance) const
{
    return SpecialCollections::collection(enumToByteArray(type), instance);
}

bool SpecialMailCollections::registerCollection(Type type, const Collection &collection)
{
    return SpecialCollections::registerCollection(enumToByteArray(type), collection);
}

bool SpecialMailCollections::unregisterCollection(const Collection &collection)
{
    if (collection != Akonadi::SpecialMailCollections::self()->defaultCollection(Akonadi::SpecialMailCollections::Trash)) {
        return SpecialCollections::unregisterCollection(collection);
    } else {
        return false;
    }
}

bool SpecialMailCollections::hasDefaultCollection(Type type) const
{
    return SpecialCollections::hasDefaultCollection(enumToByteArray(type));
}

Collection SpecialMailCollections::defaultCollection(Type type) const
{
    return SpecialCollections::defaultCollection(enumToByteArray(type));
}

void SpecialMailCollections::verifyI18nDefaultCollection(Type type)
{
    Collection collection = defaultCollection(type);
    QString defaultI18n;

    switch (type) {
    case SpecialMailCollections::Inbox:
        defaultI18n = i18nc("local mail folder", "inbox");
        break;
    case SpecialMailCollections::Outbox:
        defaultI18n = i18nc("local mail folder", "outbox");
        break;
    case SpecialMailCollections::SentMail:
        defaultI18n = i18nc("local mail folder", "sent-mail");
        break;
    case SpecialMailCollections::Trash:
        defaultI18n = i18nc("local mail folder", "trash");
        break;
    case SpecialMailCollections::Drafts:
        defaultI18n = i18nc("local mail folder", "drafts");
        break;
    case SpecialMailCollections::Templates:
        defaultI18n = i18nc("local mail folder", "templates");
        break;
    default:
        break;
    }
    if (!defaultI18n.isEmpty()) {
        if (collection.hasAttribute<Akonadi::EntityDisplayAttribute>()) {
            if (collection.attribute<Akonadi::EntityDisplayAttribute>()->displayName() != defaultI18n) {
                collection.attribute<Akonadi::EntityDisplayAttribute>()->setDisplayName(defaultI18n);
                auto job = new Akonadi::CollectionModifyJob(collection, this);
                connect(job, &Akonadi::CollectionModifyJob::result, this, &SpecialMailCollections::slotCollectionModified);
            }
        }
    }
}

void SpecialMailCollections::slotCollectionModified(KJob *job)
{
    if (job->error()) {
        qCDebug(AKONADIMIME_LOG) << " Error when we modified collection";
        return;
    }
}

SpecialMailCollections::Type SpecialMailCollections::specialCollectionType(const Akonadi::Collection &collection)
{
    if (!collection.hasAttribute<SpecialCollectionAttribute>()) {
        return Invalid;
    } else {
        return typeToEnum(collection.attribute<SpecialCollectionAttribute>()->collectionType());
    }
}

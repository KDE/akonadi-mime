/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "specialmailcollections.h"
#include "specialmailcollectionssettings.h"
#include "specialmailcollectionstesting_p.h"

#include <Akonadi/SpecialCollection>

using namespace Akonadi;

typedef SpecialMailCollectionsSettings Settings;

SpecialMailCollectionsTesting *SpecialMailCollectionsTesting::_t_self()
{
    static SpecialMailCollectionsTesting instance;
    return &instance;
}

void SpecialMailCollectionsTesting::_t_setDefaultResourceId(const QString &resourceId)
{
    Settings::self()->defaultResourceIdItem()->setValue(resourceId);
    Settings::self()->save();
}

void SpecialMailCollectionsTesting::_t_forgetFoldersForResource(const QString &resourceId)
{
    static_cast<SpecialCollections *>(SpecialMailCollections::self())->d->forgetFoldersForResource(resourceId);
}

void SpecialMailCollectionsTesting::_t_beginBatchRegister()
{
    static_cast<SpecialCollections *>(SpecialMailCollections::self())->d->beginBatchRegister();
}

void SpecialMailCollectionsTesting::_t_endBatchRegister()
{
    static_cast<SpecialCollections *>(SpecialMailCollections::self())->d->endBatchRegister();
}

int SpecialMailCollectionsTesting::_t_knownResourceCount() const
{
    return static_cast<SpecialCollections *>(SpecialMailCollections::self())->d->mFoldersForResource.count();
}

int SpecialMailCollectionsTesting::_t_knownFolderCount() const
{
    const SpecialCollectionsPrivate *d = static_cast<SpecialCollections *>(SpecialMailCollections::self())->d;
    int ret = 0;

    QHashIterator<QString, QHash<QByteArray, Collection>> resourceIt(d->mFoldersForResource);
    while (resourceIt.hasNext()) {
        resourceIt.next();

        QHashIterator<QByteArray, Collection> it(resourceIt.value());
        while (it.hasNext()) {
            it.next();
            if (it.value().isValid()) {
                ret++;
            }
        }
    }
    return ret;
}

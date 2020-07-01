/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef MOVETOTRASHCOMMAND_H
#define MOVETOTRASHCOMMAND_H

#include "commandbase.h"

#include <collection.h>
#include <item.h>

class QAbstractItemModel;
class KJob;
namespace Akonadi {
class MoveToTrashCommand : public CommandBase
{
    Q_OBJECT
public:
    MoveToTrashCommand(const QAbstractItemModel *model, const Akonadi::Item::List &msgList, QObject *parent = nullptr);
    MoveToTrashCommand(const QAbstractItemModel *model, const Akonadi::Collection::List &folders, QObject *parent = nullptr);

    void execute() override;

private Q_SLOTS:
    void slotFetchDone(KJob *job);
    void slotMoveDone(Akonadi::CommandBase::Result result);

private:
    void moveMessages();
    Akonadi::Collection trashCollectionFromResource(const Akonadi::Collection &col);
    Akonadi::Collection trashCollectionFolder();
    Akonadi::Collection findTrashFolder(const Akonadi::Collection &folder);
    Akonadi::Collection collectionFromId(Collection::Id id) const;

    Akonadi::Collection::List mFolders;
    Akonadi::Item::List mMessages;
    Akonadi::Collection::Id the_trashCollectionFolder;
    const QAbstractItemModel *mModel = nullptr;
    int mFolderListJobCount = 0;
};
}
#endif // MOVETOTRASHCOMMAND_H

/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#pragma once

#include "commandbase.h"

#include <Akonadi/Collection>
#include <Akonadi/Item>

class QAbstractItemModel;
class KJob;
namespace Akonadi
{
class MoveToTrashCommand : public CommandBase
{
    Q_OBJECT
public:
    MoveToTrashCommand(const QAbstractItemModel *model, const Akonadi::Item::List &msgList, QObject *parent = nullptr);
    MoveToTrashCommand(const QAbstractItemModel *model, const Akonadi::Collection::List &folders, QObject *parent = nullptr);

    void execute() override;

private Q_SLOTS:
    void slotMoveDone(Akonadi::CommandBase::Result result);

private:
    void slotFetchDone(KJob *job);
    void moveMessages();
    [[nodiscard]] Akonadi::Collection trashCollectionFromResource(const Akonadi::Collection &col);
    [[nodiscard]] Akonadi::Collection trashCollectionFolder();
    [[nodiscard]] Akonadi::Collection findTrashFolder(const Akonadi::Collection &folder);
    [[nodiscard]] Akonadi::Collection collectionFromId(Collection::Id id) const;

    Akonadi::Collection::List mFolders;
    Akonadi::Item::List mMessages;
    Akonadi::Collection::Id the_trashCollectionFolder;
    const QAbstractItemModel *mModel = nullptr;
    int mFolderListJobCount = 0;
};
}

/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#pragma once

#include "commandbase.h"

#include <Akonadi/AgentInstance>
#include <Akonadi/Collection>
#include <QPointer>
class QAbstractItemModel;
namespace Akonadi
{
class EmptyTrashCommand : public CommandBase
{
    Q_OBJECT

public:
    EmptyTrashCommand(const QAbstractItemModel *model, QObject *parent);
    EmptyTrashCommand(const Akonadi::Collection &folder, QObject *parent);
    void execute() override;
    void setParentWidget(QWidget *parentWidget);
protected Q_SLOTS:
    void emitResult(Akonadi::CommandBase::Result result) override;

private:
    void expunge(const Akonadi::Collection &col);
    [[nodiscard]] Akonadi::AgentInstance::List agentInstances();
    [[nodiscard]] Akonadi::Collection trashCollectionFolder();
    [[nodiscard]] Akonadi::Collection collectionFromId(Akonadi::Collection::Id id) const;
    [[nodiscard]] bool folderIsTrash(const Akonadi::Collection &col);

    const QAbstractItemModel *const mModel;
    Akonadi::Collection::Id the_trashCollectionFolder;
    Akonadi::Collection mFolder;
    QPointer<QWidget> mParentWidget;
    int mNumberOfTrashToEmpty = 0;
};
}

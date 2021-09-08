/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#pragma once

#include "akonadi-mime_export.h"
#include "commandbase.h"
#include <Akonadi/Collection>
#include <Akonadi/Item>

class KJob;
namespace Akonadi
{
class MoveCommandPrivate;
class AKONADI_MIME_EXPORT MoveCommand : public CommandBase
{
    Q_OBJECT
public:
    MoveCommand(const Akonadi::Collection &destFolder, const Akonadi::Item::List &msgList, QObject *parent = nullptr);
    ~MoveCommand() override;
    void execute() override;

private Q_SLOTS:
    void slotMoveResult(KJob *job);

private:
    MoveCommandPrivate *const d;
};
}

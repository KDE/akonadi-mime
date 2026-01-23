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

#include <memory>

class KJob;
namespace Akonadi
{
class MoveCommandPrivate;
/*!
 * \class Akonadi::MoveCommand
 * \inmodule AkonadiMime
 * \inheaderfile Akonadi/MoveCommand
 *
 * \brief The MoveCommand class
 */
class AKONADI_MIME_EXPORT MoveCommand : public CommandBase
{
    Q_OBJECT
public:
    /*!
     */
    MoveCommand(const Akonadi::Collection &destFolder, const Akonadi::Item::List &msgList, QObject *parent = nullptr);
    /*!
     */
    ~MoveCommand() override;
    /*!
     */
    void execute() override;

private:
    AKONADI_MIME_NO_EXPORT void slotMoveResult(KJob *job);
    std::unique_ptr<MoveCommandPrivate> const d;
};
}

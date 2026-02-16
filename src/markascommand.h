/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#pragma once

#include "commandbase.h"
#include "messagestatus.h"

#include <Akonadi/Collection>
#include <Akonadi/Item>

#include "akonadi-mime_export.h"

#include <memory>

class KJob;
namespace Akonadi
{
class MarkAsCommandPrivate;
/*!
 * \class Akonadi::MarkAsCommand
 * \inmodule AkonadiMime
 * \inheaderfile Akonadi/MarkAsCommand
 *
 * \brief Command to mark messages with a specific status.
 */
class AKONADI_MIME_EXPORT MarkAsCommand : public CommandBase
{
    Q_OBJECT
public:
    /*!
      Creates a new mark as command for a list of items.
      \a targetStatus The status to mark items with
      \a msgList The list of items to mark
      \a invert Whether to invert the status operation
      \a parent The parent object
    */
    MarkAsCommand(MessageStatus targetStatus, const Akonadi::Item::List &msgList, bool invert = false, QObject *parent = nullptr);
    /*!
      Creates a new mark as command for items in collections.
      \a targetStatus The status to mark items with
      \a folders The list of collections to process
      \a invert Whether to invert the status operation
      \a recursive Whether to process subfolders recursively
      \a parent The parent object
    */
    MarkAsCommand(MessageStatus targetStatus, const Akonadi::Collection::List &folders, bool invert = false, bool recursive = false, QObject *parent = nullptr);
    /*!
      Destroys the mark as command.
    */
    ~MarkAsCommand() override;
    /*!
      Executes the mark as command.
    */
    void execute() override;

private:
    AKONADI_MIME_NO_EXPORT void slotCollectionFetchDone(KJob *job);
    AKONADI_MIME_NO_EXPORT void slotFetchDone(KJob *job);
    AKONADI_MIME_NO_EXPORT void slotHelperDone(Result result);
    AKONADI_MIME_NO_EXPORT void slotModifyItemDone();
    AKONADI_MIME_NO_EXPORT void markMessages();

    std::unique_ptr<MarkAsCommandPrivate> const d;
};
}

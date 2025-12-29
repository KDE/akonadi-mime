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
 */
class AKONADI_MIME_EXPORT MarkAsCommand : public CommandBase
{
    Q_OBJECT
public:
    /*!
     */
    MarkAsCommand(MessageStatus targetStatus, const Akonadi::Item::List &msgList, bool invert = false, QObject *parent = nullptr);
    /*!
     */
    MarkAsCommand(MessageStatus targetStatus, const Akonadi::Collection::List &folders, bool invert = false, bool recursive = false, QObject *parent = nullptr);
    /*!
     */
    ~MarkAsCommand() override;
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

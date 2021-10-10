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
class AKONADI_MIME_EXPORT MarkAsCommand : public CommandBase
{
    Q_OBJECT
public:
    MarkAsCommand(MessageStatus targetStatus, const Akonadi::Item::List &msgList, bool invert = false, QObject *parent = nullptr);
    MarkAsCommand(MessageStatus targetStatus, const Akonadi::Collection::List &folders, bool invert = false, bool recursive = false, QObject *parent = nullptr);
    ~MarkAsCommand() override;
    void execute() override;

private Q_SLOTS:
    void slotCollectionFetchDone(KJob *job);
    void slotFetchDone(KJob *job);
    void slotModifyItemDone(KJob *job);

private:
    void markMessages();

    std::unique_ptr<MarkAsCommandPrivate> const d;
};
}

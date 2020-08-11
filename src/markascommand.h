/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef MARKASCOMMAND_H
#define MARKASCOMMAND_H

#include "commandbase.h"
#include "messagestatus.h"

#include <collection.h>
#include <item.h>

#include "akonadi-mime_export.h"

class KJob;
namespace Akonadi {
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
    MarkAsCommandPrivate *const d;
};
}
#endif // MARKASCOMMAND_H

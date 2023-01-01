/*
    SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"
#include "commandbase.h"
#include <Akonadi/Item>
#include <QObject>
namespace Akonadi
{
class AKONADI_MIME_EXPORT MarkAsCommandHelper : public QObject
{
    Q_OBJECT
public:
    explicit MarkAsCommandHelper(QObject *parent = nullptr);
    ~MarkAsCommandHelper() override;

    Q_REQUIRED_RESULT const Akonadi::Item::List &itemsToModify() const;
    void setItemsToModify(const Akonadi::Item::List &newItemsToModify);

    void start();

Q_SIGNALS:
    void emitResult(Akonadi::CommandBase::Result result);

private:
    void modifyMessages();
    void slotModifyItemDone(KJob *job);
    Akonadi::Item::List mItemsToModify;
    int mIndex = 0;
};
}

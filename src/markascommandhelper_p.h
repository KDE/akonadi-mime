/*
    SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"
#include "commandbase.h"
#include <Akonadi/Item>
#include <QObject>
namespace Akonadi
{
/*!
 * \class Akonadi::MarkAsCommandHelper
 * \inmodule AkonadiMime
 * \inheaderfile Akonadi/MarkAsCommandHelper
 *
 * \brief Helper class for marking messages with a status.
 *
 * \internal
 */
class AKONADI_MIME_EXPORT MarkAsCommandHelper : public QObject
{
    Q_OBJECT
public:
    /*!
      Creates a new mark as command helper.
      \a parent The parent object
    */
    explicit MarkAsCommandHelper(QObject *parent = nullptr);
    /*!
      Destroys the mark as command helper.
    */
    ~MarkAsCommandHelper() override;

    /*!
      Returns the list of items to modify.
    */
    [[nodiscard]] const Akonadi::Item::List &itemsToModify() const;
    /*!
      Sets the list of items to modify.
      \a newItemsToModify the items to modify
    */
    void setItemsToModify(const Akonadi::Item::List &newItemsToModify);

    /*!
      Starts the modification process.
    */
    void start();

Q_SIGNALS:
    /*!
      Emitted when the modification is complete.
      \a result the result of the operation
    */
    void emitResult(Akonadi::CommandBase::Result result);

private:
    void modifyMessages();
    void slotModifyItemDone(KJob *job);
    Akonadi::Item::List mItemsToModify;
    int mIndex = 0;
};
}

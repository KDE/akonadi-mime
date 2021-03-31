/*
    SPDX-FileCopyrightText: 2006 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"

#include <AkonadiCore/EntityTreeModel>

namespace Akonadi
{
/**
 * A flat self-updating message model.
 */
class AKONADI_MIME_EXPORT MessageModel : public Akonadi::EntityTreeModel
{
    Q_OBJECT

public:
    /**
      Column types.
    */
    enum Column {
        Subject, /**< Subject column. */
        Sender, /**< Sender column. */
        Receiver, /**< Receiver column. */
        Date, /**< Date column. */
        Size /**< Size column. */
    };

    /**
      Creates a new message model.

      @param parent The parent object.
    */
    explicit MessageModel(Monitor *monitor, QObject *parent = nullptr);

    /**
      Deletes the message model.
    */
    ~MessageModel() override = default;

protected:
    Q_REQUIRED_RESULT int entityColumnCount(HeaderGroup headerGroup) const override;
    Q_REQUIRED_RESULT QVariant entityHeaderData(int section, Qt::Orientation orientation, int role, HeaderGroup headerGroup) const override;
    Q_REQUIRED_RESULT QVariant entityData(const Item &item, int column, int role = Qt::DisplayRole) const override;
};
}


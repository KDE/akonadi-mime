/*
    Copyright (c) 2006 Volker Krause <vkrause@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#ifndef AKONADI_MESSAGEMODEL_H
#define AKONADI_MESSAGEMODEL_H

#include "akonadi-mime_export.h"

#include <AkonadiCore/EntityTreeModel>

namespace Akonadi {
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

#endif

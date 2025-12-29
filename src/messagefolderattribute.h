/*
    SPDX-FileCopyrightText: 2009 Kevin Ottens <ervin@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"

#include <Akonadi/Attribute>

#include <QByteArray>

#include <memory>

namespace Akonadi
{
class MessageFolderAttributePrivate;

/*!
 * \class Akonadi::MessageFolderAttribute
 * \inmodule AkonadiMime
 * \inheaderfile Akonadi/MessageFolderAttribute
 *
 * Message folder information. Used eg. by mail clients to decide how to display the content of such collections
 * \since 4.4
 */
class AKONADI_MIME_EXPORT MessageFolderAttribute : public Attribute
{
public:
    /*!
      Creates an empty folder attribute.
    */
    MessageFolderAttribute();

    /*!
      Copy constructor.
    */
    MessageFolderAttribute(const MessageFolderAttribute &other);

    /*!
      Destructor.
    */
    ~MessageFolderAttribute() override;

    /*!
      Indicates if the folder is supposed to contain mostly outbound messages.
      In such a case mail clients display the recipient address, otherwise they
      display the sender address.

      Returns true if the folder contains outbound messages
    */
    [[nodiscard]] bool isOutboundFolder() const;

    /*!
      Set if the folder should be considered as containing mostly outbound messages.
     */
    void setOutboundFolder(bool outbound);

    // reimpl.
    [[nodiscard]] QByteArray type() const override;
    MessageFolderAttribute *clone() const override;
    [[nodiscard]] QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;

private:
    std::unique_ptr<MessageFolderAttributePrivate> const d;
};
}

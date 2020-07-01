/*
    SPDX-FileCopyrightText: 2009 Kevin Ottens <ervin@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef AKONADI_MESSAGEFOLDERATTRIBUTE_H
#define AKONADI_MESSAGEFOLDERATTRIBUTE_H

#include "akonadi-mime_export.h"

#include <attribute.h>

#include <QByteArray>

namespace Akonadi {
/**
  Message folder information. Used eg. by mail clients to decide how to display the content of such collections
  @since 4.4
*/
class AKONADI_MIME_EXPORT MessageFolderAttribute : public Attribute
{
public:
    /**
      Creates an empty folder attribute.
    */
    MessageFolderAttribute();

    /**
      Copy constructor.
    */
    MessageFolderAttribute(const MessageFolderAttribute &other);

    /**
      Destructor.
    */
    ~MessageFolderAttribute();

    /**
      Indicates if the folder is supposed to contain mostly outbound messages.
      In such a case mail clients display the recipient address, otherwise they
      display the sender address.

      @return true if the folder contains outbound messages
    */
    bool isOutboundFolder() const;

    /**
      Set if the folder should be considered as containing mostly outbound messages.
     */
    void setOutboundFolder(bool outbound);

    // reimpl.
    QByteArray type() const override;
    MessageFolderAttribute *clone() const override;
    QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;

private:
    class Private;
    Private *const d;
};
}

#endif

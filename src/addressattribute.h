/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef AKONADI_ADDRESSATTRIBUTE_H
#define AKONADI_ADDRESSATTRIBUTE_H

#include "akonadi-mime_export.h"

#include <QString>
#include <QStringList>

#include <attribute.h>

namespace Akonadi {
/**
  Attribute storing the From, To, Cc, Bcc addresses of a message.

  @author Constantin Berzan <exit3219@gmail.com>
  @since 4.4
*/
class AKONADI_MIME_EXPORT AddressAttribute : public Akonadi::Attribute
{
public:
    /**
      Creates a new AddressAttribute.
    */
    explicit AddressAttribute(const QString &from = QString(), const QStringList &to = QStringList(), const QStringList &cc = QStringList(), const QStringList &bcc = QStringList());
    /**
      Destroys the AddressAttribute.
    */
    ~AddressAttribute() override;

    /* reimpl */
    AddressAttribute *clone() const override;
    QByteArray type() const override;
    QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;

    /**
      Returns the address of the sender.
    */
    QString from() const;

    /**
      Sets the address of the sender.
    */
    void setFrom(const QString &from);

    /**
      Returns the addresses of the "To:" receivers.
    */
    QStringList to() const;

    /**
      * Sets the addresses of the "To: "receivers."
      * @param to address of the receiver.
      */
    void setTo(const QStringList &to);

    /**
      Returns the addresses of the "Cc:" receivers.
    */
    QStringList cc() const;

    /**
      * Sets the addresses of the "Cc:" receivers."
      * @param cc addresses of the recievers (CC)
      */
    void setCc(const QStringList &cc);

    /**
      * Returns the addresses of the "Bcc:" receivers.
    */
    QStringList bcc() const;

    /**
    * Sets the addresses of the "Bcc:" receivers."
    * @param bcc addresses of the recievers (CC)
    */
    void setBcc(const QStringList &bcc);

private:
    class Private;
    Private *const d;
};
} // namespace Akonadi

#endif // AKONADI_ADDRESSATTRIBUTE_H

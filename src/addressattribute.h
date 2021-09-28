/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"

#include <QString>
#include <QStringList>

#include <Akonadi/Attribute>

namespace Akonadi
{
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
    explicit AddressAttribute(const QString &from = QString(),
                              const QStringList &to = QStringList(),
                              const QStringList &cc = QStringList(),
                              const QStringList &bcc = QStringList(),
                              bool dsn = false);
    /**
      Destroys the AddressAttribute.
    */
    ~AddressAttribute() override;

    /* reimpl */
    Q_REQUIRED_RESULT AddressAttribute *clone() const override;
    Q_REQUIRED_RESULT QByteArray type() const override;
    Q_REQUIRED_RESULT QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;

    /**
      Returns the address of the sender.
    */
    Q_REQUIRED_RESULT QString from() const;

    /**
      Sets the address of the sender.
    */
    void setFrom(const QString &from);

    /**
      Returns the addresses of the "To:" receivers.
    */
    Q_REQUIRED_RESULT QStringList to() const;

    /**
     * Sets the addresses of the "To: "receivers."
     * @param to address of the receiver.
     */
    void setTo(const QStringList &to);

    /**
      Returns the addresses of the "Cc:" receivers.
    */
    Q_REQUIRED_RESULT QStringList cc() const;

    /**
     * Sets the addresses of the "Cc:" receivers."
     * @param cc addresses of the receivers (CC)
     */
    void setCc(const QStringList &cc);

    /**
     * Returns the addresses of the "Bcc:" receivers.
     */
    Q_REQUIRED_RESULT QStringList bcc() const;

    /**
     * Sets the addresses of the "Bcc:" receivers."
     * @param bcc addresses of the receivers (CC)
     */
    void setBcc(const QStringList &bcc);

    void setDeliveryStatusNotification(bool b);
    Q_REQUIRED_RESULT bool deliveryStatusNotification() const;

    bool operator==(const AddressAttribute &other) const;

private:
    class Private;
    Private *const d;
};
} // namespace Akonadi


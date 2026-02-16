/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"

#include <QString>
#include <QStringList>

#include <Akonadi/Attribute>

#include <memory>

namespace Akonadi
{
class AddressAttributePrivate;

/*!
  \class Akonadi::AddressAttribute
  \inmodule AkonadiMime
  \inheaderfile Akonadi/AddressAttribute

  Attribute storing the From, To, Cc, Bcc addresses of a message.

  \author Constantin Berzan <exit3219@gmail.com>
  \since 4.4
*/
class AKONADI_MIME_EXPORT AddressAttribute : public Akonadi::Attribute
{
public:
    /*!
      Creates a new AddressAttribute.
    */
    explicit AddressAttribute(const QString &from = QString(),
                              const QStringList &to = QStringList(),
                              const QStringList &cc = QStringList(),
                              const QStringList &bcc = QStringList(),
                              bool dsn = false);
    /*!
      Destroys the AddressAttribute.
    */
    ~AddressAttribute() override;

    /*!
      Returns a copy of this attribute.
    */
    [[nodiscard]] AddressAttribute *clone() const override;
    /*!
      Returns the type of the attribute.
    */
    [[nodiscard]] QByteArray type() const override;
    /*!
      Returns the serialized representation of the attribute.
    */
    [[nodiscard]] QByteArray serialized() const override;
    /*!
      Deserializes the attribute from the given data.
      \a data the serialized data
    */
    void deserialize(const QByteArray &data) override;

    /*!
      Returns the address of the sender.
    */
    [[nodiscard]] QString from() const;

    /*!
      Sets the address of the sender.
    */
    void setFrom(const QString &from);

    /*!
      Returns the addresses of the "To:" receivers.
    */
    [[nodiscard]] QStringList to() const;

    /*!
     * Sets the addresses of the "To: "receivers."
     * \a to address of the receiver.
     */
    void setTo(const QStringList &to);

    /*!
      Returns the addresses of the "Cc:" receivers.
    */
    [[nodiscard]] QStringList cc() const;

    /*!
     * Sets the addresses of the "Cc:" receivers."
     * \a cc addresses of the receivers (CC)
     */
    void setCc(const QStringList &cc);

    /*!
     * Returns the addresses of the "Bcc:" receivers.
     */
    [[nodiscard]] QStringList bcc() const;

    /*!
     * Sets the addresses of the "Bcc:" receivers."
     * \a bcc addresses of the receivers (CC)
     */
    void setBcc(const QStringList &bcc);

    /*!
     * \brief setDeliveryStatusNotification
     */
    void setDeliveryStatusNotification(bool b);
    /*!
     */
    [[nodiscard]] bool deliveryStatusNotification() const;

    bool operator==(const AddressAttribute &other) const;

private:
    std::unique_ptr<AddressAttributePrivate> const d;
};
} // namespace Akonadi

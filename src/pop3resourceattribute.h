/*
    SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"
#include <Akonadi/Attribute>

#include <memory>

namespace Akonadi
{
class Pop3ResourceAttributePrivate;
/*!
 * \class Akonadi::Pop3ResourceAttribute
 * \inmodule AkonadiMime
 * \inheaderfile Akonadi/Pop3ResourceAttribute
 *
 * \brief The Pop3ResourceAttribute class
 */
class AKONADI_MIME_EXPORT Pop3ResourceAttribute : public Akonadi::Attribute
{
public:
    /*!
      Creates a new Pop3ResourceAttribute.
    */
    explicit Pop3ResourceAttribute();
    /*!
      Destroys the Pop3ResourceAttribute.
    */
    ~Pop3ResourceAttribute() override;

    /*!
      Returns a copy of this attribute.
    */
    Pop3ResourceAttribute *clone() const override;
    /*!
      Returns the type of the attribute.
    */
    [[nodiscard]] QByteArray type() const override;
    /*!
      Returns the serialized representation of the attribute.
    */
    [[nodiscard]] QByteArray serialized() const override;
    /*!
      Deserializes the attribute data.
      \a data the serialized data
    */
    void deserialize(const QByteArray &data) override;

    /*!
      Returns the POP3 account name.
    */
    [[nodiscard]] QString pop3AccountName() const;
    /*!
      Sets the POP3 account name.
      \a accountName the name of the POP3 account
    */
    void setPop3AccountName(const QString &accountName);

    /*!
     */
    bool operator==(const Pop3ResourceAttribute &other) const;

private:
    friend class Pop3ResourceAttributePrivate;
    std::unique_ptr<Pop3ResourceAttributePrivate> const d;
};
}

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
class NewMailNotifierAttributePrivate;
/*!
 * \class Akonadi::NewMailNotifierAttribute
 * \inmodule AkonadiMime
 * \inheaderfile Akonadi/NewMailNotifierAttribute
 *
 * \brief The NewMailNotifierAttribute class
 */
class AKONADI_MIME_EXPORT NewMailNotifierAttribute : public Akonadi::Attribute
{
public:
    /*!
      Creates a new NewMailNotifierAttribute.
    */
    explicit NewMailNotifierAttribute();
    /*!
      Destroys the NewMailNotifierAttribute.
    */
    ~NewMailNotifierAttribute() override;

    /*!
      Returns a copy of this attribute.
    */
    NewMailNotifierAttribute *clone() const override;
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
      Returns whether new mail notifications should be ignored.
    */
    [[nodiscard]] bool ignoreNewMail() const;

    /*!
      Sets whether new mail notifications should be ignored.
      \a b the ignore new mail setting
    */
    void setIgnoreNewMail(bool b);
    /*!
      Compares this attribute with another NewMailNotifierAttribute.
      Returns true if both attributes are equal.
    */
    bool operator==(const NewMailNotifierAttribute &other) const;

private:
    friend class NewMailNotifierAttributePrivate;
    std::unique_ptr<NewMailNotifierAttributePrivate> const d;
};
}

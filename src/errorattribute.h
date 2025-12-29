/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"

#include <QString>

#include <Akonadi/Attribute>

#include <memory>

namespace Akonadi
{
class ErrorAttributePrivate;

/*!
 * \class Akonadi::ErrorAttribute
 * \inmodule AkonadiMime
 * \inheaderfile Akonadi/ErrorAttribute
 *
 * \brief An Attribute to mark messages that failed to be sent.
 *
 * This attribute contains the error message encountered.
 *
 * \author Constantin Berzan <exit3219@gmail.com>
 * \since 4.4
 */
class AKONADI_MIME_EXPORT ErrorAttribute : public Akonadi::Attribute
{
public:
    /*!
     * Creates a new error attribute.
     *
     * \a msg The i18n'ed error message.
     */
    explicit ErrorAttribute(const QString &msg = QString());

    /*!
     * Destroys the error attribute.
     */
    ~ErrorAttribute() override;

    /*!
     * Returns the i18n'ed error message.
     */
    [[nodiscard]] QString message() const;

    /*!
     * Sets the i18n'ed error message.
     */
    void setMessage(const QString &msg);

    /* reimpl */
    ErrorAttribute *clone() const override;
    [[nodiscard]] QByteArray type() const override;
    [[nodiscard]] QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;

private:
    std::unique_ptr<ErrorAttributePrivate> const d;
};
}

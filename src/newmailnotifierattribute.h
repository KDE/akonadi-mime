/*
    SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"

#include <Akonadi/Attribute>

#include <memory>

namespace Akonadi
{
class NewMailNotifierAttributePrivate;
class AKONADI_MIME_EXPORT NewMailNotifierAttribute : public Akonadi::Attribute
{
public:
    explicit NewMailNotifierAttribute();
    ~NewMailNotifierAttribute() override;

    /* reimpl */
    NewMailNotifierAttribute *clone() const override;
    [[nodiscard]] QByteArray type() const override;
    [[nodiscard]] QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;

    [[nodiscard]] bool ignoreNewMail() const;
    void setIgnoreNewMail(bool b);
    bool operator==(const NewMailNotifierAttribute &other) const;

private:
    friend class NewMailNotifierAttributePrivate;
    std::unique_ptr<NewMailNotifierAttributePrivate> const d;
};
}

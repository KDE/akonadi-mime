/*
    SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"
#include <Akonadi/Attribute>

#include <memory>

namespace Akonadi
{
class Pop3ResourceAttributePrivate;
class AKONADI_MIME_EXPORT Pop3ResourceAttribute : public Akonadi::Attribute
{
public:
    explicit Pop3ResourceAttribute();
    ~Pop3ResourceAttribute() override;

    /* reimpl */
    Pop3ResourceAttribute *clone() const override;
    [[nodiscard]] QByteArray type() const override;
    [[nodiscard]] QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;

    [[nodiscard]] QString pop3AccountName() const;
    void setPop3AccountName(const QString &accountName);

    bool operator==(const Pop3ResourceAttribute &other) const;

private:
    friend class Pop3ResourceAttributePrivate;
    std::unique_ptr<Pop3ResourceAttributePrivate> const d;
};
}

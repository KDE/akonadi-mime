/*
    SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"
#include <AkonadiCore/Attribute>

#include <QScopedPointer>

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
    QByteArray type() const override;
    QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;

    QString pop3AccountName() const;
    void setPop3AccountName(const QString &accountName);

    bool operator==(const Pop3ResourceAttribute &other) const;

private:
    friend class Pop3ResourceAttributePrivate;
    QScopedPointer<Pop3ResourceAttributePrivate> const d;
};
}

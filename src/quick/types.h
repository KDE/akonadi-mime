// SPDX-FileCopyrightText: 2025 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: LGPL-2.0-or-later

#pragma once

#include <qqmlregistration.h>

#include "../specialmailcollections.h"

class QQmlEngine;
class QJSEngine;

namespace Akonadi
{
namespace Quick
{

class SpecialMailCollectionsForeign : public QObject
{
    Q_OBJECT
    QML_FOREIGN(Akonadi::SpecialMailCollections)
    QML_NAMED_ELEMENT(SpecialMailCollections)
    QML_SINGLETON

public:
    static Akonadi::SpecialMailCollections *create(QQmlEngine *, QJSEngine *)
    {
        return Akonadi::SpecialMailCollections::self();
    }
};
}
}

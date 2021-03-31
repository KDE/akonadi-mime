/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mimeprivate_export.h"
#include "specialmailcollections.h"

namespace Akonadi
{
/**
  @internal
  Class that exposes SpecialMailCollections' private methods for use in unit tests.
  HACK Is there a better way to do this?
*/
class AKONADI_MIME_TEST_EXPORT SpecialMailCollectionsTesting
{
public:
    static SpecialMailCollectionsTesting *_t_self();
    void _t_setDefaultResourceId(const QString &resourceId);
    void _t_forgetFoldersForResource(const QString &resourceId);
    void _t_beginBatchRegister();
    void _t_endBatchRegister();
    int _t_knownResourceCount() const;
    int _t_knownFolderCount() const;
};
} // namespace Akonadi


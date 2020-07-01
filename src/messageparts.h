/*
    SPDX-FileCopyrightText: 2006 Volker Krause <vkrause@kde.org>
                  2007 Till Adam <adam@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef AKONADI_MESSAGEPARTS_H
#define AKONADI_MESSAGEPARTS_H

#include "akonadi-mime_export.h"

namespace Akonadi {
/**
 * @short Contains predefined part identifiers.
 *
 * This namespace contains identifiers of item parts that are used for
 * handling email items.
 */
namespace MessagePart {
/**
 * The part identifier for envelope parts.
 */
AKONADI_MIME_EXPORT extern const char Envelope[];

/**
 * The part identifier for the main body part.
 */
AKONADI_MIME_EXPORT extern const char Body[];

/**
 * The part identifier for the header part.
 */
AKONADI_MIME_EXPORT extern const char Header[];
}
}

#endif

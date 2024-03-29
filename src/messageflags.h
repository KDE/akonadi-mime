/*
 * SPDX-FileCopyrightText: 2010 Klaralvdalens Datakonsult AB a KDAB Group company, <info@kdab.com>
 * SPDX-FileCopyrightText: 2010 Leo Franchi <lfranchi@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "akonadi-mime_export.h"

namespace KMime
{
class Message;
}

namespace Akonadi
{
class Item;

/**
 * @short Contains predefined message flag identifiers.
 *
 * This namespace contains identifiers of message flags that
 *  are used internally in the Akonadi server.
 */
namespace MessageFlags
{
/**
 * The flag for a message being seen (i.e. opened by user).
 */
AKONADI_MIME_EXPORT extern const char Seen[];

/**
 * The flag for a message being deleted by the user.
 */
AKONADI_MIME_EXPORT extern const char Deleted[];

/**
 * The flag for a message being replied to by the user.
 * @deprecated use Replied instead.
 */
AKONADI_MIME_EXPORT extern const char Answered[];

/**
 * The flag for a message being marked as flagged.
 */
AKONADI_MIME_EXPORT extern const char Flagged[];

/**
 * The flag for a message being marked with an error.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char HasError[];

/**
 * The flag for a message being marked as having an attachment.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char HasAttachment[];

/**
 * The flag for a message being marked as having an invitation.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char HasInvitation[];

/**
 * The flag for a message being marked as sent.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char Sent[];

/**
 * The flag for a message being marked as queued.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char Queued[];

/**
 * The flag for a message being marked as replied.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char Replied[];

/**
 * The flag for a message being marked as forwarded.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char Forwarded[];

/**
 * The flag for a message being marked as action item to act on.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char ToAct[];

/**
 * The flag for a message being marked as watched.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char Watched[];

/**
 * The flag for a message being marked as ignored.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char Ignored[];

/**
 * The flag for a message being marked as signed.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char Signed[];

/**
 * The flag for a message being marked as encrypted.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char Encrypted[];

/**
 * The flag for a message being marked as spam.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char Spam[];

/**
 * The flag for a message being marked as ham.
 * @since 4.6
 */
AKONADI_MIME_EXPORT extern const char Ham[];

/**
 * Copies all message flags from a KMime::Message object
 * into an Akonadi::Item object
 * @since 4.14.6
 */
AKONADI_MIME_EXPORT void copyMessageFlags(KMime::Message &from, Akonadi::Item &to);
}
}

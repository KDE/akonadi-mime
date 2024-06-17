/*
 * SPDX-FileCopyrightText: 2010 Klaralvdalens Datakonsult AB a KDAB Group company, <info@kdab.com>
 * SPDX-FileCopyrightText: 2010 Leo Franchi <lfranchi@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "messageflags.h"

#include <Akonadi/Item>
#include <KMime/Message>
#include <KMime/Util>

const char Akonadi::MessageFlags::Seen[] = "\\SEEN";
const char Akonadi::MessageFlags::Deleted[] = "\\DELETED";
const char Akonadi::MessageFlags::Answered[] = "\\ANSWERED";
const char Akonadi::MessageFlags::Flagged[] = "\\FLAGGED";
const char Akonadi::MessageFlags::HasError[] = "$ERROR";
const char Akonadi::MessageFlags::HasAttachment[] = "$ATTACHMENT";
const char Akonadi::MessageFlags::HasInvitation[] = "$INVITATION";
const char Akonadi::MessageFlags::Sent[] = "$SENT";
const char Akonadi::MessageFlags::Queued[] = "$QUEUED";
const char Akonadi::MessageFlags::Replied[] = "$REPLIED";
const char Akonadi::MessageFlags::Forwarded[] = "$FORWARDED";
const char Akonadi::MessageFlags::ToAct[] = "$TODO";
const char Akonadi::MessageFlags::Watched[] = "$WATCHED";
const char Akonadi::MessageFlags::Ignored[] = "$IGNORED";
const char Akonadi::MessageFlags::Signed[] = "$SIGNED";
const char Akonadi::MessageFlags::Encrypted[] = "$ENCRYPTED";
const char Akonadi::MessageFlags::Spam[] = "$JUNK";
const char Akonadi::MessageFlags::Ham[] = "$NOTJUNK";

void Akonadi::MessageFlags::copyMessageFlags(KMime::Message &message, Akonadi::Item &item)
{
    if (KMime::isSigned(&message)) {
        item.setFlag(Akonadi::MessageFlags::Signed);
    }

    if (KMime::isEncrypted(&message)) {
        item.setFlag(Akonadi::MessageFlags::Encrypted);
    }

    if (KMime::hasInvitation(&message)) {
        item.setFlag(Akonadi::MessageFlags::HasInvitation);
    }

    if (KMime::hasAttachment(&message)) {
        item.setFlag(Akonadi::MessageFlags::HasAttachment);
    }
}

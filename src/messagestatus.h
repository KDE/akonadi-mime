/*  -*- mode: C++ -*-
    This file is part of Akonadi.
    SPDX-FileCopyrightText: 2005 Andreas Gungl <a.gungl@gmx.de>
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Leo Franchi <lfranchi@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "akonadi-mime_export.h"
#include <QMetaType>
#include <QSet>

class QString;
class QDebug;

namespace Akonadi
{
//---------------------------------------------------------------------------
/*!
  \class Akonadi::MessageStatus
  \inmodule AkonadiMime
  \inheaderfile Akonadi/MessageStatus

  \brief Akonadi KMime Message Status.
  \author Andreas Gungl <a.gungl@gmx.de>

  The class encapsulates the handling of the different flags
  which describe the status of a message.
  The flags themselves are not intended to be used outside this class.

  In the status pairs Watched/Ignored and Spam/Ham, there both
  values can't be set at the same time, however they can
  be unset at the same time.

  Note that this class does not sync with the Akonadi storage. It is
  used as an in-memory helper when manipulating Akonadi items.

  \since 4.6.
*/
class AKONADI_MIME_EXPORT MessageStatus
{
    Q_GADGET
    Q_PROPERTY(bool isOfUnknownStatus READ isOfUnknownStatus CONSTANT)
    Q_PROPERTY(bool isRead READ isRead WRITE setRead)
    Q_PROPERTY(bool isDeleted READ isDeleted WRITE setDeleted)
    Q_PROPERTY(bool isReplied READ isReplied WRITE setReplied)
    Q_PROPERTY(bool isForwarded READ isForwarded WRITE setForwarded)
    Q_PROPERTY(bool isQueued READ isQueued WRITE setQueued)
    Q_PROPERTY(bool isSent READ isSent WRITE setSent)
    Q_PROPERTY(bool isImportant READ isImportant WRITE setImportant)
    Q_PROPERTY(bool isWatched READ isWatched WRITE setWatched)
    Q_PROPERTY(bool isIgnored READ isIgnored WRITE setIgnored)
    Q_PROPERTY(bool isSpam READ isSpam WRITE setSpam)
    Q_PROPERTY(bool isHam READ isHam WRITE setHam)
    Q_PROPERTY(bool isToAct READ isToAct WRITE setToAct)
    Q_PROPERTY(bool hasAttachment READ hasAttachment WRITE setHasAttachment)
    Q_PROPERTY(bool hasInvitation READ hasInvitation WRITE setHasInvitation)
    Q_PROPERTY(bool isEncrypted READ isEncrypted WRITE setEncrypted)
    Q_PROPERTY(bool isSigned READ isSigned WRITE setSigned)
    Q_PROPERTY(bool hasError READ hasError WRITE setHasError)

public:
    /*! Constructor - sets status initially to unknown. */
    MessageStatus();

    /*! Compare the status with that from another instance.
        Returns true if the stati are equal, false if different.
        \a other message status to compare with current object
    */
    bool operator==(MessageStatus other) const;

    /*! Compare the status with that from another instance.
        Returns true if the stati are equal, false if different.
        \a other message status to compare with current object
    */
    bool operator!=(MessageStatus other) const;

    /*! Check, if some of the flags in the status match
        with those flags from another instance.
        Returns true if at least one flag is set in both stati.
        \a other message status to compare objects' flags
    */
    bool operator&(MessageStatus other) const;

    /*! Clear all status flags, this resets to unknown. */
    void clear();

    /*! Set / add stati described by another MessageStatus object.
        This can be used to merge in multiple stati at once without
        using the single setter methods.
        However, internally the setters are used anyway to ensure the
        integrity of the resulting status.
        \a other message status to set
    */
    void set(MessageStatus other);

    /*! Toggle one or more stati described by another MessageStatus object.
        Internally the setters are used to ensure the integrity of the
        resulting status.
        \a other message status to toggle
    */
    void toggle(MessageStatus other);

    /* ----- getters ----------------------------------------------------- */

    /*! Check for Unknown status.
        Returns true if status is unknown.
    */
    [[nodiscard]] bool isOfUnknownStatus() const;

    /*! Check for Read status. Note that ignored messages are read.
        Returns true if status is read.
    */
    [[nodiscard]] bool isRead() const;

    /*! Check for Deleted status.
        Returns true if status is deleted.
    */
    [[nodiscard]] bool isDeleted() const;

    /*! Check for Replied status.
        Returns true if status is replied.
    */
    [[nodiscard]] bool isReplied() const;

    /*! Check for Forwarded status.
        Returns true if status is forwarded.
    */
    [[nodiscard]] bool isForwarded() const;

    /*! Check for Queued status.
        Returns true if status is queued.
    */
    [[nodiscard]] bool isQueued() const;

    /*! Check for Sent status.
        Returns true if status is sent.
    */
    [[nodiscard]] bool isSent() const;

    /*! Check for Important status.
        Returns true if status is important.
    */
    [[nodiscard]] bool isImportant() const;

    /*! Check for Watched status.
        Returns true if status is watched.
    */
    [[nodiscard]] bool isWatched() const;

    /*! Check for Ignored status.
        Returns true if status is ignored.
    */
    [[nodiscard]] bool isIgnored() const;

    /*! Check for ToAct status.
        Returns true if status is action item.
    */
    [[nodiscard]] bool isToAct() const;

    /*! Check for Spam status.
        Returns true if status is spam.
    */
    [[nodiscard]] bool isSpam() const;

    /*! Check for Ham status.
        Returns true if status is not spam.
    */
    [[nodiscard]] bool isHam() const;

    /*! Check for Attachment status.
        Returns true if status indicates an attachment.
    */
    [[nodiscard]] bool hasAttachment() const;

    /*! Check for Invitation status.
        Returns true if status indicates an invitation.
    */
    [[nodiscard]] bool hasInvitation() const;

    /*! Check for Signed status.
        Returns true if status is signed.
    */
    [[nodiscard]] bool isSigned() const;

    /*! Check for Encrypted status.
        Returns true if status is encrypted.
    */
    [[nodiscard]] bool isEncrypted() const;

    /*! Check for error status.
        Returns true if status indicates an error.
    */
    [[nodiscard]] bool hasError() const;

    /* ----- setters ----------------------------------------------------- */

    /*!
     * Set the status to read
     * \a read new read status
     */
    void setRead(bool read = true);

    /*! Set the status for deleted.
        \a deleted Set (true) or unset (false) this status flag.
    */
    void setDeleted(bool deleted = true);

    /*! Set the status for replied.
        \a replied Set (true) or unset (false) this status flag.
    */
    void setReplied(bool replied = true);

    /*! Set the status for forwarded.
        \a forwarded Set (true) or unset (false) this status flag.
    */
    void setForwarded(bool forwarded = true);

    /*! Set the status for queued.
        \a queued Set (true) or unset (false) this status flag.
    */
    void setQueued(bool queued = true);

    /*! Set the status for sent.
        \a sent Set (true) or unset (false) this status flag.
    */
    void setSent(bool sent = true);

    /*! Set the status for important.
        \a important Set (true) or unset (false) this status flag.
    */
    void setImportant(bool important = true);

    /*! Set the status to watched.
        \a watched Set (true) or unset (false) this status flag.
    */
    void setWatched(bool watched = true);

    /*! Set the status to ignored.
        \a ignored Set (true) or unset (false) this status flag.
    */
    void setIgnored(bool ignored = true);

    /*! Set the status to action item.
        \a toAct Set (true) or unset (false) this status flag.
    */
    void setToAct(bool toAct = true);

    /*! Set the status to spam.
        \a spam Set (true) or unset (false) this status flag.
    */
    void setSpam(bool spam = true);

    /*! Set the status to not spam.
        \a ham Set (true) or unset (false) this status flag.
    */
    void setHam(bool ham = true);

    /*! Set the status for an attachment.
        \a hasAttachment Set (true) or unset (false) this status flag.
    */
    void setHasAttachment(bool hasAttachment = true);

    /*! Set the status for an invitation.
        \a hasInvitation Set (true) or unset (false) this status flag.
    */
    void setHasInvitation(bool hasInvitation = true);

    /*! Set the status to signed.
        \a value Set (true) or unset (false) this status flag.
    */
    void setSigned(bool value = true);

    /*! Set the status to encrypted.
        \a value Set (true) or unset (false) this status flag.
    */
    void setEncrypted(bool value = true);

    /*! Set the status to error.
        \a value Set (true) or unset (false) this status flag.
    */
    void setHasError(bool value = true);

    /* ----- state representation  --------------------------------------- */

    /*! Get the status as a whole e.g. for storage in an index.
     D on't manipulte the *index via this value, this bypasses
     all integrity checks in the setter methods.
     Returns The status encoded in bits.
     */
    [[nodiscard]] qint32 toQInt32() const;

    /*! Set the status as a whole e.g. for reading from an index.
        Don't manipulte the index via this value, this bypasses
        all integrity checks in the setter methods.
        \a status The status encoded in bits to be set in this instance.
    */
    void fromQInt32(qint32 status);

    /*! Convert the status to a string representation.
        Returns A string containing coded uppercase letters
                which describe the status.

        Note This code is legacy for the KMail1 indexes
    */
    [[nodiscard]] QString statusStr() const;

    /*! Set the status based on a string representation.
        \a aStr The status string to be analyzed.
                    Normally it is a string obtained using
                    getStatusStr().

        Note This code is legacy for the KMail1 indexes
    */
    void setStatusFromStr(const QString &aStr);

    /*! Get the status as a whole e.g. for storage as IMAP flags.
        Returns The status encoded in flags.
    */
    [[nodiscard]] QSet<QByteArray> statusFlags() const;

    /*! Set the status as a whole e.g. for reading from IMAP flags.
        \a flags set of flags for status as a whole
    */
    void setStatusFromFlags(const QSet<QByteArray> &flags);

    /* ----- static accessors to simple states --------------------------- */

    /*! Return a special status that expresses Unread.
        This status can only be used for comparison with other states.
        Unread is special because search is performed using a simple bit-mask comparison, which
        cannot return nonzero for an *un*set flag. This special flag works around that.
    */
    static const MessageStatus statusUnread();

    /*! Return a predefined status initialized as Read as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Read.
    */
    static const MessageStatus statusRead();

    /*! Return a predefined status initialized as Deleted as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Deleted.
    */
    static const MessageStatus statusDeleted();

    /*! Return a predefined status initialized as Replied as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Replied.
    */
    static const MessageStatus statusReplied();

    /*! Return a predefined status initialized as Forwarded as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Forwarded.
    */
    static const MessageStatus statusForwarded();

    /*! Return a predefined status initialized as Queued as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Queued.
    */
    static const MessageStatus statusQueued();

    /*! Return a predefined status initialized as Sent as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Sent.
    */
    static const MessageStatus statusSent();

    /*! Return a predefined status initialized as Important as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Important.
    */
    static const MessageStatus statusImportant();

    /*! Return a predefined status initialized as Watched as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Watched.
    */
    static const MessageStatus statusWatched();

    /*! Return a predefined status initialized as Ignored as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Ignored.
    */
    static const MessageStatus statusIgnored();

    /*! Return a predefined status initialized as Action Item as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as ToAct.
    */
    static const MessageStatus statusToAct();

    /*! Return a predefined status initialized as Spam as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Spam.
    */
    static const MessageStatus statusSpam();

    /*! Return a predefined status initialized as Ham as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Ham.
    */
    static const MessageStatus statusHam();

    /*! Return a predefined status initialized as Attachment as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Attachment.
    */
    static const MessageStatus statusHasAttachment();

    /*! Return a predefined status initialized as Invitation as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Invitation.
    */
    static const MessageStatus statusHasInvitation();

    /*! Return a predefined status initialized as Signed as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Signed.
    */
    static const MessageStatus statusSigned();

    /*! Return a predefined status initialized as Encrypted as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Encrypted.
    */
    static const MessageStatus statusEncrypted();

    /*! Return a predefined status initialized as Error as is useful
        e.g. when providing a state for comparison.
        Returns A reference to a status instance initialized as Error.
    */
    static const MessageStatus statusHasError();

private:
    quint32 mStatus;
};
} // namespace Akonadi

AKONADI_MIME_EXPORT QDebug operator<<(QDebug d, const Akonadi::MessageStatus &t);

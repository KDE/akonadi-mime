/*
    SPDX-FileCopyrightText: 2007 Till Adam <adam@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "akonadi_serializer_mail.h"
#include "akonadi_serializer_mail_debug.h"
#include "messageparts.h"

#include <QDataStream>
#include <QIODevice>
#include <qplugin.h>

#include <KMime/Message>

#include <Akonadi/Item>
#include <private/imapparser_p.h>

using namespace Akonadi;
using namespace KMime;

QString StringPool::sharedValue(const QString &value)
{
    QMutexLocker lock(&m_mutex);
    QSet<QString>::const_iterator it = m_pool.constFind(value);
    if (it != m_pool.constEnd()) {
        return *it;
    }
    m_pool.insert(value);
    return value;
}

// v1 format or older
[[nodiscard]] static std::optional<KMime::Types::Mailbox> parseMailbox(const QByteArray &addrString, int version, StringPool &pool)
{
    QVarLengthArray<QByteArray, 16> addr;
    ImapParser::parseParenthesizedList(addrString, addr);
    if (addr.count() != 4) {
        qCWarning(AKONADI_SERIALIZER_MAIL_LOG) << "Error parsing envelope address field: " << addrString;
        return {};
    }
    KMime::Types::Mailbox addrField;
    if (version == 0) {
        addrField.setNameFrom7Bit(addr[0]);
    } else if (version == 1) {
        addrField.setName(pool.sharedValue(QString::fromUtf8(addr[0])));
    }
    KMime::Types::AddrSpec addrSpec;
    addrSpec.localPart = pool.sharedValue(QString::fromUtf8(addr[2]));
    addrSpec.domain = pool.sharedValue(QString::fromUtf8(addr[3]));
    addrField.setAddress(addrSpec);
    return addrField;
}

static void parseAddrList(const QVarLengthArray<QByteArray, 16> &addrList, KMime::Headers::Generics::MailboxList *hdr, int version, StringPool &pool)
{
    const int count = addrList.count();
    QList<KMime::Types::Mailbox> mboxList;
    mboxList.reserve(count);

    for (int i = 0; i < count; ++i) {
        auto addrField = parseMailbox(addrList[i], version, pool);
        if (addrField) {
            mboxList.push_back(std::move(*addrField));
        }
    }

    hdr->setMailboxes(mboxList);
}

static void parseAddrList(const QVarLengthArray<QByteArray, 16> &addrList, KMime::Headers::Generics::SingleMailbox *hdr, int version, StringPool &pool)
{
    const int count = addrList.count();
    if (count != 1) {
        return;
    }
    QList<KMime::Types::Mailbox> mboxList;
    mboxList.reserve(count);

    for (int i = 0; i < count; ++i) {
        auto addrField = parseMailbox(addrList[i], version, pool);
        if (addrField) {
            mboxList.push_back(std::move(*addrField));
        }
    }

    hdr->setMailbox(mboxList[0]);
}

static void parseAddrList(const QVarLengthArray<QByteArray, 16> &addrList, KMime::Headers::Generics::AddressList *hdr, int version, StringPool &pool)
{
    const int count = addrList.count();
    QList<KMime::Types::Address> addressList;
    addressList.reserve(count);

    for (int i = 0; i < count; ++i) {
        auto addrField = parseMailbox(addrList[i], version, pool);
        if (addrField) {
            KMime::Types::Address addr;
            addr.mailboxList.push_back(std::move(*addrField));
            addressList.push_back(std::move(addr));
        }
    }

    hdr->setAddressList(addressList);
}

// v2 format
[[nodiscard]] static KMime::Types::Mailbox parseMailbox(QDataStream &stream, StringPool &pool)
{
    static thread_local QString str;
    KMime::Types::Mailbox mbox;
    KMime::Types::AddrSpec addrSpec;
    stream >> str;
    mbox.setName(pool.sharedValue(str));
    stream >> str;
    addrSpec.localPart = pool.sharedValue(str);
    stream >> str;
    addrSpec.domain = pool.sharedValue(str);
    mbox.setAddress(addrSpec);
    return mbox;
}

template<typename T>
static void parseMailboxList(QDataStream &stream, T hdrFunc, QSharedPointer<KMime::Message> &msg, int version, StringPool &pool)
{
    Q_UNUSED(version)

    int count = 0;
    stream >> count;
    if (count == 0) {
        return;
    }
    QList<KMime::Types::Mailbox> mboxList;
    mboxList.reserve(count);

    for (int i = 0; i < count; ++i) {
        mboxList.push_back(parseMailbox(stream, pool));
    }

    (msg.get()->*(hdrFunc))(true)->setMailboxes(mboxList);
}

template<typename T>
static void parseSingleMailbox(QDataStream &stream, T hdrFunc, QSharedPointer<KMime::Message> &msg, int version, StringPool &pool)
{
    Q_UNUSED(version)

    int count = 0;
    stream >> count;
    if (count == 0) {
        return;
    }

    // we need to consume the entire stream, even if it contains more than one element...
    for (int i = 0; i < count; ++i) {
        if (!i) {
            (msg.get()->*(hdrFunc))(true)->setMailbox(parseMailbox(stream, pool));
        }
    }
}

template<typename T>
static void parseAddrList(QDataStream &stream, T hdrFunc, QSharedPointer<KMime::Message> &msg, int version, StringPool &pool)
{
    Q_UNUSED(version)

    int count = 0;
    stream >> count;
    if (count == 0) {
        return;
    }
    QList<KMime::Types::Address> addrList;
    addrList.reserve(count);

    for (int i = 0; i < count; ++i) {
        KMime::Types::Address addr;
        addr.mailboxList.push_back(parseMailbox(stream, pool));
        addrList.push_back(std::move(addr));
    }

    (msg.get()->*(hdrFunc))(true)->setAddressList(addrList);
}

bool SerializerPluginMail::deserialize(Item &item, const QByteArray &label, QIODevice &data, int version)
{
    if (label != MessagePart::Body && label != MessagePart::Envelope && label != MessagePart::Header) {
        return false;
    }

    QSharedPointer<KMime::Message> msg;
    if (!item.hasPayload()) {
        auto m = new Message();
        msg = QSharedPointer<KMime::Message>(m);
        item.setPayload(msg);
    } else {
        msg = item.payload<QSharedPointer<KMime::Message>>();
    }

    if (label == MessagePart::Body) {
        QByteArray buffer = data.readAll();
        if (buffer.isEmpty()) {
            return true;
        }
        msg->setContent(buffer);
        msg->parse();
    } else if (label == MessagePart::Header) {
        QByteArray buffer = data.readAll();
        if (buffer.isEmpty()) {
            return true;
        }
        if (msg->body().isEmpty() && msg->contents().isEmpty()) {
            msg->setHead(buffer);
            msg->parse();
        }
    } else if (label == MessagePart::Envelope) {
        if (version <= 1) {
            QByteArray buffer = data.readAll();
            if (buffer.isEmpty()) {
                return true;
            }
            QVarLengthArray<QByteArray, 16> env;
            ImapParser::parseParenthesizedList(buffer, env);
            if (env.count() < 10) {
                qCWarning(AKONADI_SERIALIZER_MAIL_LOG) << "Akonadi KMime Deserializer: Got invalid envelope: " << buffer;
                return false;
            }
            Q_ASSERT(env.count() >= 10);
            // date
            msg->date()->from7BitString(env[0]);
            // subject
            msg->subject()->from7BitString(env[1]);
            // from
            QVarLengthArray<QByteArray, 16> addrList;
            ImapParser::parseParenthesizedList(env[2], addrList);
            if (!addrList.isEmpty()) {
                parseAddrList(addrList, msg->from(), version, m_stringPool);
            }
            // sender
            ImapParser::parseParenthesizedList(env[3], addrList);
            if (!addrList.isEmpty()) {
                parseAddrList(addrList, msg->sender(), version, m_stringPool);
            }
            // reply-to
            ImapParser::parseParenthesizedList(env[4], addrList);
            if (!addrList.isEmpty()) {
                parseAddrList(addrList, msg->replyTo(), version, m_stringPool);
            }
            // to
            ImapParser::parseParenthesizedList(env[5], addrList);
            if (!addrList.isEmpty()) {
                parseAddrList(addrList, msg->to(), version, m_stringPool);
            }
            // cc
            ImapParser::parseParenthesizedList(env[6], addrList);
            if (!addrList.isEmpty()) {
                parseAddrList(addrList, msg->cc(), version, m_stringPool);
            }
            // bcc
            ImapParser::parseParenthesizedList(env[7], addrList);
            if (!addrList.isEmpty()) {
                parseAddrList(addrList, msg->bcc(), version, m_stringPool);
            }
            // in-reply-to
            if (!env[8].isEmpty()) {
                msg->inReplyTo()->from7BitString(env[8]);
            }
            // message id
            msg->messageID()->from7BitString(env[9]);
            // references
            if (env.count() > 10) {
                msg->references()->from7BitString(env[10]);
            }
        } else if (version == 2) {
            QDataStream stream(&data);
            QDateTime dt;
            QString str;

            stream >> dt;
            msg->date()->setDateTime(dt);
            stream >> str;
            msg->subject()->fromUnicodeString(str);

            QString inReplyTo;
            stream >> inReplyTo;
            if (!inReplyTo.isEmpty()) {
                msg->inReplyTo()->fromUnicodeString(inReplyTo);
            }
            stream >> str;
            msg->messageID()->fromUnicodeString(str);
            stream >> str;
            if (!str.isEmpty()) {
                if (str == inReplyTo) {
                    msg->references()->fromIdent(*msg->inReplyTo());
                } else {
                    msg->references()->fromUnicodeString(str);
                }
            }

            parseMailboxList(stream, qOverload<bool>(&KMime::Message::from), msg, version, m_stringPool);
            parseSingleMailbox(stream, qOverload<bool>(&KMime::Message::sender), msg, version, m_stringPool);
            parseAddrList(stream, qOverload<bool>(&KMime::Message::replyTo), msg, version, m_stringPool);
            parseAddrList(stream, qOverload<bool>(&KMime::Message::to), msg, version, m_stringPool);
            parseAddrList(stream, qOverload<bool>(&KMime::Message::cc), msg, version, m_stringPool);
            parseAddrList(stream, qOverload<bool>(&KMime::Message::bcc), msg, version, m_stringPool);

            if (stream.status() == QDataStream::ReadCorruptData || stream.status() == QDataStream::ReadPastEnd) {
                qCWarning(AKONADI_SERIALIZER_MAIL_LOG) << "Akonadi KMime Deserializer: Got invalid envelope";
                return false;
            }
        }
    }

    return true;
}

template<typename T>
static void serializeAddrList(QDataStream &stream, T *hdr)
{
    if (!hdr) {
        stream << (qint32)0;
        return;
    }

    const QList<KMime::Types::Mailbox> mb = hdr->mailboxes();
    stream << (qint32)mb.size();
    for (const KMime::Types::Mailbox &mbox : mb) {
        stream << mbox.name() << mbox.addrSpec().localPart << mbox.addrSpec().domain;
    }
}

static void serializeAddr(QDataStream &stream, const KMime::Headers::Generics::SingleMailbox *hdr)
{
    if (!hdr) {
        stream << (qint32)0;
        return;
    }

    stream << (qint32)1;
    const auto mbox = hdr->mailbox();
    stream << mbox.name() << mbox.addrSpec().localPart << mbox.addrSpec().domain;
}

template<typename T>
static QString asUnicodeString(T *hdr)
{
    return hdr ? hdr->asUnicodeString() : QString();
}

void SerializerPluginMail::serialize(const Item &item, const QByteArray &label, QIODevice &data, int &version)
{
    version = 1;

    auto m = item.payload<QSharedPointer<const KMime::Message>>();
    if (label == MessagePart::Body) {
        data.write(m->encodedContent());
    } else if (label == MessagePart::Envelope) {
        version = 2;
        QDataStream stream(&data);
        const auto dtHdr = m->date();
        stream << (dtHdr ? dtHdr->dateTime() : QDateTime()) << asUnicodeString(m->subject()) << asUnicodeString(m->inReplyTo())
               << asUnicodeString(m->messageID()) << asUnicodeString(m->references());
        serializeAddrList(stream, m->from());
        serializeAddr(stream, m->sender());
        serializeAddrList(stream, m->replyTo());
        serializeAddrList(stream, m->to());
        serializeAddrList(stream, m->cc());
        serializeAddrList(stream, m->bcc());
    } else if (label == MessagePart::Header) {
        data.write(m->head());
    }
}

QSet<QByteArray> SerializerPluginMail::parts(const Item &item) const
{
    QSet<QByteArray> set;

    if (!item.hasPayload<QSharedPointer<KMime::Message>>()) {
        return set;
    }

    auto msg = item.payload<QSharedPointer<KMime::Message>>();
    if (!msg) {
        return set;
    }

    // FIXME: we really want "has any header" here, but the kmime api doesn't offer that yet
    if (msg->hasContent() || msg->hasHeader("Message-ID")) {
        set << MessagePart::Envelope << MessagePart::Header;
        if (!msg->body().isEmpty() || !msg->contents().isEmpty()) {
            set << MessagePart::Body;
        }
    }
    return set;
}

QString SerializerPluginMail::extractGid(const Item &item) const
{
    if (!item.hasPayload<QSharedPointer<KMime::Message>>()) {
        return {};
    }
    const auto msg = item.payload<QSharedPointer<const KMime::Message>>();
    const KMime::Headers::MessageID *mid = msg->messageID();
    if (mid) {
        return mid->asUnicodeString();
    } else if (KMime::Headers::Base *uid = msg->headerByType("X-Akonotes-UID")) {
        return uid->asUnicodeString();
    }
    return {};
}

#include "moc_akonadi_serializer_mail.cpp"

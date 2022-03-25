/*
    SPDX-FileCopyrightText: 2007 Till Adam <adam@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QMutex>
#include <QObject>

#include <Akonadi/GidExtractorInterface>
#include <Akonadi/ItemSerializerPlugin>

namespace Akonadi
{
/**
 * Levare QString implicit sharing to decrease memory consumption.
 *
 * This class is thread safe. Apparently required for usage in
 * legacy KRes compat bridges.
 */
class StringPool
{
public:
    /**
     * Lookup @p value in the pool and return the known value
     * to reuse it and leverage the implicit sharing. Otherwise
     * add the value to the pool and return it again.
     */
    QString sharedValue(const QString &value);

private:
    QMutex m_mutex;
    QSet<QString> m_pool;
};

class SerializerPluginMail : public QObject, public ItemSerializerPlugin, public GidExtractorInterface
{
    Q_OBJECT
    Q_INTERFACES(Akonadi::ItemSerializerPlugin Akonadi::GidExtractorInterface)
    Q_PLUGIN_METADATA(IID "org.kde.akonadi.SerializerPluginMail")
public:
    bool deserialize(Item &item, const QByteArray &label, QIODevice &data, int version) override;
    void serialize(const Item &item, const QByteArray &label, QIODevice &data, int &version) override;
    QSet<QByteArray> parts(const Item &item) const override;
    QString extractGid(const Item &item) const override;

private:
    StringPool m_stringPool;
};
}

/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>
    SPDX-FileCopyrightText: 2012 Dan Vrátil <dvratil@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#pragma once

#include <Akonadi/Collection>
#include <Akonadi/Job>

#include "akonadi-mime_export.h"

#include <memory>

namespace Akonadi
{
class RemoveDuplicatesJobPrivate;

/*!
 * \class Akonadi::RemoveDuplicatesJob
 * \inmodule AkonadiMime
 * \inheaderfile Akonadi/RemoveDuplicatesJob
 *
 * \brief Job that finds and removes duplicate messages in given collection
 *
 * This jobs compares all messages in given collections by their Message-Id
 * headers and hashes of their bodies and removes duplicates.
 *
 * \since 4.10
 */
class AKONADI_MIME_EXPORT RemoveDuplicatesJob : public Akonadi::Job
{
    Q_OBJECT

public:
    /*!
     * Creates a new job that will remove duplicates in \a folder.
     *
     * \a folder The folder where to search for duplicates
     * \a parent The parent object
     */
    explicit RemoveDuplicatesJob(const Akonadi::Collection &folder, QObject *parent = nullptr);

    /*!
     * Creates a new job that will remove duplicates in all \a folders.
     *
     * \a folders Folders where to search for duplicates
     * \a parent The parent object
     */
    RemoveDuplicatesJob(const Akonadi::Collection::List &folders, QObject *parent);

    /*!
     * Destroys the job.
     */
    ~RemoveDuplicatesJob() override;

protected:
    void doStart() override;
    bool doKill() override;

private:
    friend class RemoveDuplicatesJobPrivate;
    std::unique_ptr<RemoveDuplicatesJobPrivate> const d;
};
} /* namespace Akonadi */

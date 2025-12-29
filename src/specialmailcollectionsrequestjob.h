/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"
#include "specialmailcollections.h"

#include <Akonadi/SpecialCollectionsRequestJob>

#include <memory>

namespace Akonadi
{
class SpecialMailCollectionsRequestJobPrivate;

/*!
 * \brief A job to request SpecialMailCollections.
 *
 * Use this job to request the SpecialMailCollections you need. You can request both
 * default SpecialMailCollections and SpecialMailCollections in a given resource. The default
 * SpecialMailCollections resource is created when the first default SpecialCollection is
 * requested, but if a SpecialCollection in a custom resource is requested, this
 * job expects that resource to exist already.
 *
 * If the folders you requested already exist, this job simply succeeds.
 * Otherwise, it creates the required collections and registers them with
 * SpecialMailCollections.
 *
 * Example:
 *
 * \code
 *
 * SpecialMailCollectionsRequestJob *job = new SpecialMailCollectionsRequestJob( this );
 * job->requestDefaultCollection( SpecialMailCollections::Outbox );
 * connect( job, SIGNAL(result(KJob*)),
 *          this, SLOT(requestDone(KJob*)) );
 *
 * ...
 *
 * MyClass::requestDone( KJob *job )
 * {
 *   if ( job->error() )
 *     return;
 *
 *   SpecialMailCollectionsRequestJob *requestJob = qobject_cast<SpecialMailCollectionsRequestJob*>( job );
 *
 *   const Collection collection = requestJob->collection();
 *   ...
 * }
 *
 * \endcode
 *
 * \author Constantin Berzan <exit3219@gmail.com>
 * \since 4.4
 */
class AKONADI_MIME_EXPORT SpecialMailCollectionsRequestJob : public SpecialCollectionsRequestJob
{
    Q_OBJECT

public:
    /*!
     * Creates a new special mail collections request job.
     */
    explicit SpecialMailCollectionsRequestJob(QObject *parent = nullptr);

    /*!
     * Destroys the special mail collections request job.
     */
    ~SpecialMailCollectionsRequestJob() override;

    /*!
     * Requests a special mail collection of the given \a type in the default resource.
     */
    void requestDefaultCollection(SpecialMailCollections::Type type);

    /*!
     * Requests a special mail collection of the given \a type in the given resource \a instance.
     */
    void requestCollection(SpecialMailCollections::Type type, const AgentInstance &instance);

private:
    friend class SpecialMailCollectionsRequestJobPrivate;

    std::unique_ptr<SpecialMailCollectionsRequestJobPrivate> const d;
};
} // namespace Akonadi

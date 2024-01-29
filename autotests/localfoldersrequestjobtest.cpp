/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localfoldersrequestjobtest.h"

#include "collectionpathresolver.h"

#include <QSignalSpy>

#include <QDebug>

#include "../src/specialmailcollectionstesting_p.h"
#include "specialcollectionattribute.h"
#include "specialcollections.h"
#include <Akonadi/AgentInstance>
#include <Akonadi/AgentManager>
#include <Akonadi/CollectionCreateJob>
#include <Akonadi/CollectionDeleteJob>
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionModifyJob>
#include <QStandardPaths>
#include <akonadi/qtest_akonadi.h>
#include <control.h>
#include <specialmailcollections.h>
#include <specialmailcollectionsrequestjob.h>

using namespace Akonadi;

void LocalFoldersRequestJobTest::initTestCase()
{
    qRegisterMetaType<Akonadi::AgentInstance>();

    QVERIFY(Control::start());
    QTest::qWait(1000);

    SpecialMailCollections *smc = SpecialMailCollections::self();
    SpecialMailCollectionsTesting *smct = SpecialMailCollectionsTesting::_t_self();
    Q_ASSERT(smc);
    Q_ASSERT(smct);
    Q_UNUSED(smc)

    // No one has created the default resource.  LF has no folders.
    QCOMPARE(smct->_t_knownResourceCount(), 0);
    QCOMPARE(smct->_t_knownFolderCount(), 0);
}

void LocalFoldersRequestJobTest::testRequestWithNoDefaultResourceExisting()
{
    SpecialMailCollections *smc = SpecialMailCollections::self();
    SpecialMailCollectionsTesting *smct = SpecialMailCollectionsTesting::_t_self();
    Q_ASSERT(smc);
    Q_ASSERT(smct);
    QSignalSpy spy(smc, &SpecialMailCollections::collectionsChanged);
    QSignalSpy defSpy(smc, &SpecialMailCollections::defaultCollectionsChanged);
    QVERIFY(spy.isValid());
    QVERIFY(defSpy.isValid());

    // Initially the default maildir does not exist.
    QVERIFY(!QFile::exists(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1StringView("/local-mail")));

    // Request some default folders.
    {
        SpecialMailCollectionsRequestJob *rjob = new SpecialMailCollectionsRequestJob(this);
        rjob->requestDefaultCollection(SpecialMailCollections::Outbox);
        rjob->requestDefaultCollection(SpecialMailCollections::Drafts);
        AKVERIFYEXEC(rjob);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(defSpy.count(), 1);
        QCOMPARE(smct->_t_knownResourceCount(), 1);
        QCOMPARE(smct->_t_knownFolderCount(), 3); // Outbox, Drafts, and Root.
        QVERIFY(smc->hasDefaultCollection(SpecialMailCollections::Outbox));
        QVERIFY(smc->hasDefaultCollection(SpecialMailCollections::Drafts));
        QVERIFY(smc->hasDefaultCollection(SpecialMailCollections::Root));
    }

    // The maildir should exist now.
    QVERIFY(QFile::exists(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1StringView("/local-mail")));
}

void LocalFoldersRequestJobTest::testRequestWithDefaultResourceAlreadyExisting()
{
    SpecialMailCollections *smc = SpecialMailCollections::self();
    SpecialMailCollectionsTesting *smct = SpecialMailCollectionsTesting::_t_self();
    Q_ASSERT(smc);
    Q_ASSERT(smct);
    QSignalSpy spy(smc, &SpecialMailCollections::collectionsChanged);
    QSignalSpy defSpy(smc, &SpecialMailCollections::defaultCollectionsChanged);
    QVERIFY(spy.isValid());
    QVERIFY(defSpy.isValid());

    // Prerequisites (from testRequestWithNoDefaultResourceExisting()).
    QVERIFY(QFile::exists(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1StringView("/local-mail")));
    QVERIFY(!smc->hasDefaultCollection(SpecialMailCollections::Inbox));
    QVERIFY(smc->hasDefaultCollection(SpecialMailCollections::Outbox));
    const Collection oldOutbox = smc->defaultCollection(SpecialMailCollections::Outbox);

    // Request some default folders.
    {
        SpecialMailCollectionsRequestJob *rjob = new SpecialMailCollectionsRequestJob(this);
        rjob->requestDefaultCollection(SpecialMailCollections::Outbox); // Exists previously.
        rjob->requestDefaultCollection(SpecialMailCollections::Inbox); // Must be created.
        AKVERIFYEXEC(rjob);
        QCOMPARE(spy.count(), 1);
        QCOMPARE(defSpy.count(), 1);
        QCOMPARE(smct->_t_knownResourceCount(), 1);
        QCOMPARE(smct->_t_knownFolderCount(), 4); // Inbox, Outbox, Drafts, and Root.
        QVERIFY(smc->hasDefaultCollection(SpecialMailCollections::Inbox));
        QVERIFY(smc->hasDefaultCollection(SpecialMailCollections::Outbox));
        QVERIFY(smc->hasDefaultCollection(SpecialMailCollections::Drafts));
        QVERIFY(smc->hasDefaultCollection(SpecialMailCollections::Root));
    }

    // This should be untouched.
    QCOMPARE(smc->defaultCollection(SpecialMailCollections::Outbox), oldOutbox);
}

void LocalFoldersRequestJobTest::testMixedRequest()
{
    SpecialMailCollections *smc = SpecialMailCollections::self();
    SpecialMailCollectionsTesting *smct = SpecialMailCollectionsTesting::_t_self();
    Q_ASSERT(smc);
    Q_ASSERT(smct);
    QSignalSpy spy(smc, &SpecialMailCollections::collectionsChanged);
    QSignalSpy defSpy(smc, &SpecialMailCollections::defaultCollectionsChanged);
    QVERIFY(spy.isValid());
    QVERIFY(defSpy.isValid());

    // Get our knut collection.
    Collection res1;
    {
        CollectionPathResolver *resolver = new CollectionPathResolver(QStringLiteral("res1"), this);
        QVERIFY(resolver->exec());
        res1 = Collection(resolver->collection());
        CollectionFetchJob *fjob = new CollectionFetchJob(res1, CollectionFetchJob::Base, this);
        AKVERIFYEXEC(fjob);
        Q_ASSERT(fjob->collections().count() == 1);
        res1 = fjob->collections().first();
        QVERIFY(res1.isValid());
        QVERIFY(!res1.resource().isEmpty());
    }

    // Create a LocalFolder in the knut resource.
    Collection knutOutbox;
    {
        knutOutbox.setName(QLatin1StringView("my_outbox"));
        knutOutbox.setParentCollection(res1);
        qDebug() << res1;
        knutOutbox.addAttribute(new SpecialCollectionAttribute("outbox"));
        CollectionCreateJob *cjob = new CollectionCreateJob(knutOutbox, this);
        AKVERIFYEXEC(cjob);
        knutOutbox = cjob->collection();
    }

    // Prerequisites (from the above two functions).
    QVERIFY(QFile::exists(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1StringView("/local-mail")));
    QVERIFY(!smc->hasDefaultCollection(SpecialMailCollections::SentMail));
    QVERIFY(smc->hasDefaultCollection(SpecialMailCollections::Outbox));
    const Collection oldOutbox = smc->defaultCollection(SpecialMailCollections::Outbox);

    // Request some folders, both in our default resource and in the knut resource.
    {
        SpecialMailCollectionsRequestJob *rjob = new SpecialMailCollectionsRequestJob(this);
        rjob->requestDefaultCollection(SpecialMailCollections::Outbox); // Exists previously.
        rjob->requestDefaultCollection(SpecialMailCollections::SentMail); // Must be created.
        rjob->requestCollection(SpecialMailCollections::Outbox,
                                AgentManager::self()->instance(res1.resource())); // Exists previously, but unregistered with LF.
        rjob->requestCollection(SpecialMailCollections::SentMail, AgentManager::self()->instance(res1.resource())); // Must be created.
        AKVERIFYEXEC(rjob);
        QCOMPARE(spy.count(), 2); // Default resource and knut resource.
        QCOMPARE(defSpy.count(), 1);
        QCOMPARE(smct->_t_knownResourceCount(), 2);
        QVERIFY(smc->hasDefaultCollection(SpecialMailCollections::Outbox));
        QVERIFY(smc->hasDefaultCollection(SpecialMailCollections::SentMail));
        QVERIFY(smc->hasCollection(SpecialMailCollections::Outbox, AgentManager::self()->instance(res1.resource())));
        QVERIFY(smc->hasCollection(SpecialMailCollections::SentMail, AgentManager::self()->instance(res1.resource())));
    }

    // These should be untouched.
    QCOMPARE(smc->defaultCollection(SpecialMailCollections::Outbox), oldOutbox);
    QCOMPARE(smc->collection(SpecialMailCollections::Outbox, AgentManager::self()->instance(res1.resource())), knutOutbox);
}

QTEST_AKONADIMAIN(LocalFoldersRequestJobTest)

#include "moc_localfoldersrequestjobtest.cpp"

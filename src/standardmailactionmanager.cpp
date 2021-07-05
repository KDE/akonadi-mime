/*
    This file is part of Akonadi Contact.

    Copyright (c) 2009 - 2010 Tobias Koenig <tokoe@kde.org>
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "standardmailactionmanager.h"

#include "akonadi_mime_debug.h"
#include "emptytrashcommand.h"
#include "markascommand.h"
#include "movetotrashcommand.h"
#include "removeduplicatesjob.h"
#include "specialmailcollections.h"
#include "util_p.h"

#include <agentfilterproxymodel.h>
#include <agentinstance.h>
#include <agentinstancecreatejob.h>
#include <agentmanager.h>
#include <agenttypedialog.h>
#include <collectionstatistics.h>
#include <entitytreemodel.h>
#include <mimetypechecker.h>
#include <subscriptiondialog.h>

#include <kmime/kmime_message.h>
#include <messagestatus.h>

#include <KActionCollection>
#include <QAction>

#include <KLocalizedString>

#include <QItemSelectionModel>

using namespace Akonadi;

class Q_DECL_HIDDEN StandardMailActionManager::Private
{
public:
    Private(KActionCollection *actionCollection, QWidget *parentWidget, StandardMailActionManager *parent)
        : mActionCollection(actionCollection)
        , mParentWidget(parentWidget)
        , mParent(parent)
    {
        mGenericManager = new StandardActionManager(actionCollection, parentWidget);

        QObject::connect(
            mGenericManager,
            &StandardActionManager::selectionsChanged,
            mParent,
            [this](const Collection::List &selectedCollections, const Collection::List &selectedFavoriteCollections, const Item::List &selectedItems) {
                // Optimization: pass along the lists to avoid recalculating them here
                updateActions(selectedCollections, selectedFavoriteCollections, selectedItems);
            });

        QObject::connect(mGenericManager, &StandardActionManager::actionStateUpdated, mParent, &StandardMailActionManager::actionStateUpdated);

        mGenericManager->setMimeTypeFilter(QStringList() << KMime::Message::mimeType());
        mGenericManager->setCapabilityFilter(QStringList() << QStringLiteral("Resource"));
    }

    ~Private()
    {
        delete mGenericManager;
    }

    void updateGenericAllActions()
    {
        updateGenericAction(StandardActionManager::CreateCollection);
        updateGenericAction(StandardActionManager::CopyCollections);
        updateGenericAction(StandardActionManager::DeleteCollections);
        updateGenericAction(StandardActionManager::SynchronizeCollections);
        updateGenericAction(StandardActionManager::CollectionProperties);
        updateGenericAction(StandardActionManager::CopyItems);
        updateGenericAction(StandardActionManager::Paste);
        updateGenericAction(StandardActionManager::DeleteItems);
        updateGenericAction(StandardActionManager::ManageLocalSubscriptions);
        updateGenericAction(StandardActionManager::AddToFavoriteCollections);
        updateGenericAction(StandardActionManager::RemoveFromFavoriteCollections);
        updateGenericAction(StandardActionManager::RenameFavoriteCollection);
        updateGenericAction(StandardActionManager::CopyCollectionToMenu);
        updateGenericAction(StandardActionManager::CopyItemToMenu);
        updateGenericAction(StandardActionManager::MoveItemToMenu);
        updateGenericAction(StandardActionManager::MoveCollectionToMenu);
        updateGenericAction(StandardActionManager::CutItems);
        updateGenericAction(StandardActionManager::CutCollections);
        updateGenericAction(StandardActionManager::CreateResource);
        updateGenericAction(StandardActionManager::DeleteResources);
        updateGenericAction(StandardActionManager::ResourceProperties);
        updateGenericAction(StandardActionManager::SynchronizeResources);
        updateGenericAction(StandardActionManager::ToggleWorkOffline);
        updateGenericAction(StandardActionManager::CopyCollectionToDialog);
        updateGenericAction(StandardActionManager::MoveCollectionToDialog);
        updateGenericAction(StandardActionManager::CopyItemToDialog);
        updateGenericAction(StandardActionManager::MoveItemToDialog);
        updateGenericAction(StandardActionManager::SynchronizeCollectionsRecursive);
        updateGenericAction(StandardActionManager::MoveCollectionsToTrash);
        updateGenericAction(StandardActionManager::MoveItemsToTrash);
        updateGenericAction(StandardActionManager::RestoreCollectionsFromTrash);
        updateGenericAction(StandardActionManager::RestoreItemsFromTrash);
        updateGenericAction(StandardActionManager::MoveToTrashRestoreCollection);
        updateGenericAction(StandardActionManager::MoveToTrashRestoreCollectionAlternative);
        updateGenericAction(StandardActionManager::MoveToTrashRestoreItem);
        updateGenericAction(StandardActionManager::MoveToTrashRestoreItemAlternative);
        updateGenericAction(StandardActionManager::SynchronizeFavoriteCollections);
    }

    void updateGenericAction(StandardActionManager::Type type)
    {
        switch (type) {
        case Akonadi::StandardActionManager::CreateCollection: {
            QAction *act = mGenericManager->action(Akonadi::StandardActionManager::CreateCollection);
            act->setText(i18n("Add Folder..."));
            act->setWhatsThis(i18n("Add a new folder to the currently selected account."));
            setHelpText(act, i18n("Add a new folder to the currently selected account."));

            mGenericManager->setContextText(StandardActionManager::CreateCollection, StandardActionManager::DialogTitle, i18nc("@title:window", "New Folder"));

            mGenericManager->setContextText(StandardActionManager::CreateCollection,
                                            StandardActionManager::ErrorMessageText,
                                            ki18n("Could not create folder: %1"));

            mGenericManager->setContextText(StandardActionManager::CreateCollection, StandardActionManager::ErrorMessageTitle, i18n("Folder creation failed"));
            break;
        }
        case Akonadi::StandardActionManager::CopyCollections:
            mGenericManager->setActionText(Akonadi::StandardActionManager::CopyCollections, ki18np("Copy Folder", "Copy %1 Folders"));
            mGenericManager->action(Akonadi::StandardActionManager::CopyCollections)->setWhatsThis(i18n("Copy the selected folders to the clipboard."));
            setHelpText(mGenericManager->action(Akonadi::StandardActionManager::CopyCollections), i18n("Copy the selected folders to the clipboard."));
            mGenericManager->setContextText(StandardActionManager::CollectionProperties,
                                            StandardActionManager::DialogTitle,
                                            ki18nc("@title:window", "Properties of Folder %1"));
            break;
        case Akonadi::StandardActionManager::DeleteCollections:
            mGenericManager->setActionText(Akonadi::StandardActionManager::DeleteCollections, ki18np("Delete Folder", "Delete %1 Folders"));
            mGenericManager->action(Akonadi::StandardActionManager::DeleteCollections)->setWhatsThis(i18n("Delete the selected folders from the account."));
            setHelpText(mGenericManager->action(Akonadi::StandardActionManager::DeleteCollections), i18n("Delete the selected folders from the account."));
            mGenericManager->setContextText(StandardActionManager::DeleteCollections,
                                            StandardActionManager::MessageBoxText,
                                            ki18np("Do you really want to delete this folder and all its sub-folders?",
                                                   "Do you really want to delete %1 folders and all their sub-folders?"));

            mGenericManager->setContextText(StandardActionManager::DeleteCollections,
                                            StandardActionManager::MessageBoxTitle,
                                            ki18ncp("@title:window", "Delete folder?", "Delete folders?"));

            mGenericManager->setContextText(StandardActionManager::DeleteCollections,
                                            StandardActionManager::ErrorMessageText,
                                            ki18n("Could not delete folder: %1"));

            mGenericManager->setContextText(StandardActionManager::DeleteCollections, StandardActionManager::ErrorMessageTitle, i18n("Folder deletion failed"));

            break;
        case Akonadi::StandardActionManager::SynchronizeCollections:
            mGenericManager->setActionText(Akonadi::StandardActionManager::SynchronizeCollections, ki18np("Update Folder", "Update Folders"));

            mGenericManager->action(Akonadi::StandardActionManager::SynchronizeCollections)->setWhatsThis(i18n("Update the contents of the selected folders."));
            setHelpText(mGenericManager->action(Akonadi::StandardActionManager::SynchronizeCollections), i18n("Update the contents of the selected folders."));
            break;
        case Akonadi::StandardActionManager::CutCollections:
            mGenericManager->setActionText(Akonadi::StandardActionManager::CutCollections, ki18np("Cut Folder", "Cut %1 Folders"));
            mGenericManager->action(Akonadi::StandardActionManager::CutCollections)->setWhatsThis(i18n("Cut the selected folders from the account."));
            setHelpText(mGenericManager->action(Akonadi::StandardActionManager::CutCollections), i18n("Cut the selected folders from the account."));
            break;
        case Akonadi::StandardActionManager::CollectionProperties:
            mGenericManager->action(Akonadi::StandardActionManager::CollectionProperties)->setText(i18n("Folder Properties"));
            mGenericManager->action(Akonadi::StandardActionManager::CollectionProperties)
                ->setWhatsThis(i18n("Open a dialog to edit the properties of the selected folder."));
            setHelpText(mGenericManager->action(Akonadi::StandardActionManager::CollectionProperties),
                        i18n("Open a dialog to edit the properties of the selected folder."));
            break;
        case Akonadi::StandardActionManager::CopyItems:
            mGenericManager->setActionText(Akonadi::StandardActionManager::CopyItems, ki18np("Copy Message", "Copy %1 Messages"));
            mGenericManager->action(Akonadi::StandardActionManager::CopyItems)->setWhatsThis(i18n("Copy the selected messages to the clipboard."));
            setHelpText(mGenericManager->action(Akonadi::StandardActionManager::CopyItems), i18n("Copy the selected messages to the clipboard."));
            break;
        case Akonadi::StandardActionManager::DeleteItems:
            mGenericManager->setActionText(Akonadi::StandardActionManager::DeleteItems, ki18np("Delete Message", "Delete %1 Messages"));
            mGenericManager->action(Akonadi::StandardActionManager::DeleteItems)->setWhatsThis(i18n("Delete the selected messages from the folder."));
            setHelpText(mGenericManager->action(Akonadi::StandardActionManager::DeleteItems), i18n("Delete the selected messages from the folder."));
            mGenericManager->setContextText(StandardActionManager::DeleteItems,
                                            StandardActionManager::MessageBoxText,
                                            ki18np("Do you really want to delete the selected message?", "Do you really want to delete %1 messages?"));

            mGenericManager->setContextText(StandardActionManager::DeleteItems,
                                            StandardActionManager::MessageBoxTitle,
                                            ki18ncp("@title:window", "Delete Message?", "Delete Messages?"));

            mGenericManager->setContextText(StandardActionManager::DeleteItems, StandardActionManager::ErrorMessageText, ki18n("Could not delete message: %1"));

            mGenericManager->setContextText(StandardActionManager::DeleteItems, StandardActionManager::ErrorMessageTitle, i18n("Message deletion failed"));
            break;
        case Akonadi::StandardActionManager::CutItems:
            mGenericManager->setActionText(Akonadi::StandardActionManager::CutItems, ki18np("Cut Message", "Cut %1 Messages"));
            mGenericManager->action(Akonadi::StandardActionManager::CutItems)->setWhatsThis(i18n("Cut the selected message from the folder."));
            setHelpText(mGenericManager->action(Akonadi::StandardActionManager::CutItems), i18n("Cut the selected message from the folder."));
            break;
        case Akonadi::StandardActionManager::CreateResource:
            mGenericManager->action(Akonadi::StandardActionManager::CreateResource)->setText(i18n("Add &Account..."));
            mGenericManager->action(Akonadi::StandardActionManager::CreateResource)
                ->setWhatsThis(i18n("Add a new account<p>"
                                    "You will be presented with a dialog where you can select "
                                    "the type of the account that shall be added.</p>"));
            mGenericManager->setContextText(StandardActionManager::CreateResource, StandardActionManager::DialogTitle, i18nc("@title:window", "Add Account"));

            mGenericManager->setContextText(StandardActionManager::CreateResource,
                                            StandardActionManager::ErrorMessageText,
                                            ki18n("Could not create account: %1"));

            mGenericManager->setContextText(StandardActionManager::CreateResource, StandardActionManager::ErrorMessageTitle, i18n("Account creation failed"));
            break;
        case Akonadi::StandardActionManager::DeleteResources:
            mGenericManager->setActionText(Akonadi::StandardActionManager::DeleteResources, ki18np("&Delete Account", "&Delete %1 Accounts"));
            mGenericManager->action(Akonadi::StandardActionManager::DeleteResources)
                ->setWhatsThis(i18n("Delete the selected accounts<p>"
                                    "The currently selected accounts will be deleted, "
                                    "along with all the messages they contain.</p>"));
            mGenericManager->setContextText(StandardActionManager::DeleteResources,
                                            StandardActionManager::MessageBoxText,
                                            ki18np("Do you really want to delete this account?", "Do you really want to delete %1 accounts?"));

            mGenericManager->setContextText(StandardActionManager::DeleteResources,
                                            StandardActionManager::MessageBoxTitle,
                                            ki18ncp("@title:window", "Delete Account?", "Delete Accounts?"));
            break;
        case Akonadi::StandardActionManager::ResourceProperties:
            mGenericManager->action(Akonadi::StandardActionManager::ResourceProperties)->setText(i18n("Account Properties..."));
            mGenericManager->action(Akonadi::StandardActionManager::ResourceProperties)
                ->setWhatsThis(i18n("Open a dialog to edit properties of the selected account."));
            break;
        case Akonadi::StandardActionManager::SynchronizeResources:
            mGenericManager->setActionText(Akonadi::StandardActionManager::SynchronizeResources, ki18np("Update Account", "Update %1 Accounts"));
            mGenericManager->action(Akonadi::StandardActionManager::SynchronizeResources)
                ->setWhatsThis(i18n("Updates the content of all folders of the selected accounts."));
            break;
        case Akonadi::StandardActionManager::SynchronizeCollectionsRecursive:
            mGenericManager->setActionText(Akonadi::StandardActionManager::SynchronizeCollectionsRecursive,
                                           ki18np("Update This Folder and All Its Subfolders", "Update These Folders and All Their Subfolders"));

            mGenericManager->action(Akonadi::StandardActionManager::SynchronizeCollectionsRecursive)
                ->setWhatsThis(i18n("Update the contents of the selected folders and all their subfolders."));
            setHelpText(mGenericManager->action(Akonadi::StandardActionManager::SynchronizeCollectionsRecursive),
                        i18n("Update the contents of the selected folders and all their subfolders."));
            break;
        case Akonadi::StandardActionManager::Paste:
            mGenericManager->setContextText(StandardActionManager::Paste, StandardActionManager::ErrorMessageText, ki18n("Could not paste message: %1"));

            mGenericManager->setContextText(StandardActionManager::Paste, StandardActionManager::ErrorMessageTitle, i18n("Paste failed"));
            break;
        default:
            break;
        }
    }

    void setHelpText(QAction *act, const QString &text)
    {
        act->setStatusTip(text);
        act->setToolTip(text);
        if (act->whatsThis().isEmpty()) {
            act->setWhatsThis(text);
        }
    }

    void updateActions()
    {
        const Akonadi::Item::List selectedItems = mGenericManager->selectedItems();
        const Akonadi::Collection::List selectedCollections = mGenericManager->selectedCollections();
        updateActions(selectedCollections, {}, selectedItems);
    }

    void updateActionsFromSelectedItems(const Item::List &selectedItems)
    {
        const bool itemIsSelected = !selectedItems.isEmpty();
        if (itemIsSelected) {
            bool allMarkedAsImportant = true;
            bool allMarkedAsRead = true;
            bool allMarkedAsUnread = true;
            bool allMarkedAsActionItem = true;

            for (const Akonadi::Item &item : selectedItems) {
                Akonadi::MessageStatus status;
                status.setStatusFromFlags(item.flags());
                if (!status.isImportant()) {
                    allMarkedAsImportant = false;
                }
                if (!status.isRead()) {
                    allMarkedAsRead = false;
                } else {
                    allMarkedAsUnread = false;
                }
                if (!status.isToAct()) {
                    allMarkedAsActionItem = false;
                }
            }

            QAction *action = mActions.value(Akonadi::StandardMailActionManager::MarkMailAsRead);
            if (action) {
                updateMarkAction(action, allMarkedAsRead);
                action->setEnabled(!allMarkedAsRead);
            }

            action = mActions.value(Akonadi::StandardMailActionManager::MarkMailAsUnread);
            if (action) {
                updateMarkAction(action, allMarkedAsUnread);
                action->setEnabled(!allMarkedAsUnread);
            }

            action = mActions.value(Akonadi::StandardMailActionManager::MarkMailAsImportant);
            if (action) {
                action->setCheckable(true);
                updateMarkAction(action, allMarkedAsImportant);
                action->setChecked(allMarkedAsImportant);
                if (allMarkedAsImportant) {
                    action->setText(i18n("Remove Important Mark"));
                } else {
                    action->setText(i18n("&Mark Message as Important"));
                }
                action->setEnabled(true);
            }

            action = mActions.value(Akonadi::StandardMailActionManager::MarkMailAsActionItem);
            if (action) {
                action->setCheckable(true);
                updateMarkAction(action, allMarkedAsActionItem);
                action->setChecked(allMarkedAsActionItem);
                if (allMarkedAsActionItem) {
                    action->setText(i18n("Remove Action Item Mark"));
                } else {
                    action->setText(i18n("&Mark Message as Action Item"));
                }
                action->setEnabled(true);
            }
        } else {
            QAction *action = mActions.value(Akonadi::StandardMailActionManager::MarkMailAsRead);
            if (action) {
                action->setEnabled(false);
            }

            action = mActions.value(Akonadi::StandardMailActionManager::MarkMailAsImportant);
            if (action) {
                action->setEnabled(false);
            }

            action = mActions.value(Akonadi::StandardMailActionManager::MarkMailAsActionItem);
            if (action) {
                action->setEnabled(false);
            }
        }
    }

    void updateActions(const Collection::List &selectedCollections, const Collection::List &selectedFavoriteCollections, const Item::List &selectedItems)
    {
        Q_UNUSED(selectedFavoriteCollections)
        const bool itemIsSelected = !selectedItems.isEmpty();
        const bool collectionIsSelected = !selectedCollections.isEmpty();
        updateActionsFromSelectedItems(selectedItems);

        bool enableMarkAllAsRead = false;
        bool enableMarkAllAsUnread = false;
        bool canDeleteItem = true;
        bool isSystemFolder = false;
        if (collectionIsSelected) {
            for (const Collection &collection : std::as_const(selectedCollections)) {
                if (collection.isValid()) {
                    const Akonadi::CollectionStatistics stats = collection.statistics();
                    if (!enableMarkAllAsRead) {
                        enableMarkAllAsRead = (stats.unreadCount() > 0);
                    }
                    if (!enableMarkAllAsUnread) {
                        enableMarkAllAsUnread = (stats.count() != stats.unreadCount());
                    }
                    if (canDeleteItem) {
                        canDeleteItem = collection.rights() & Akonadi::Collection::CanDeleteItem;
                    }
                    if (!isSystemFolder) {
                        isSystemFolder = (collection == SpecialMailCollections::self()->defaultCollection(SpecialMailCollections::Inbox)
                                          || collection == SpecialMailCollections::self()->defaultCollection(SpecialMailCollections::Outbox)
                                          || collection == SpecialMailCollections::self()->defaultCollection(SpecialMailCollections::SentMail)
                                          || collection == SpecialMailCollections::self()->defaultCollection(SpecialMailCollections::Trash)
                                          || collection == SpecialMailCollections::self()->defaultCollection(SpecialMailCollections::Drafts)
                                          || collection == SpecialMailCollections::self()->defaultCollection(SpecialMailCollections::Templates));
                    }
                    // We will not change after that.
                    if (enableMarkAllAsRead && enableMarkAllAsUnread && !canDeleteItem && isSystemFolder) {
                        break;
                    }
                }
            }
        }
        QAction *action = nullptr;
        if (isSystemFolder) {
            action = mGenericManager->action(StandardActionManager::DeleteCollections);
            if (action) {
                action->setEnabled(false);
            }
        }

        action = mActions.value(Akonadi::StandardMailActionManager::MoveToTrash);
        if (action) {
            action->setEnabled(itemIsSelected && canDeleteItem);
        }
        action = mActions.value(Akonadi::StandardMailActionManager::RemoveDuplicates);
        if (action) {
            action->setEnabled(canDeleteItem);
        }

        action = mActions.value(Akonadi::StandardMailActionManager::MarkAllMailAsRead);
        if (action) {
            action->setEnabled(enableMarkAllAsRead);
        }

        action = mActions.value(Akonadi::StandardMailActionManager::MarkAllMailAsUnread);
        if (action) {
            action->setEnabled(enableMarkAllAsUnread);
        }

        Q_EMIT mParent->actionStateUpdated();
    }

    void updateMarkAction(QAction *action, bool allMarked)
    {
        QByteArray data = action->data().toByteArray();
        if (allMarked) {
            if (!data.startsWith('!')) {
                data.prepend('!');
            }
        } else {
            if (data.startsWith('!')) {
                data.remove(0, 1);
            }
        }
        action->setData(data);
    }

    void markItemsAs(QByteArray typeStr, const Akonadi::Item::List &items, bool checkIntercept = true)
    {
        qCDebug(AKONADIMIME_LOG) << "Mark mail as: " << typeStr;

        bool invert = false;
        if (typeStr.startsWith('!')) {
            invert = true;
            typeStr.remove(0, 1);
        }

        Akonadi::MessageStatus targetStatus;
        targetStatus.setStatusFromStr(QLatin1String(typeStr));

        StandardMailActionManager::Type type = MarkMailAsRead;
        if (typeStr == "U") {
            type = MarkMailAsUnread;
            targetStatus.setRead(true);
            invert = true;
        } else if (typeStr == "K") {
            type = MarkMailAsActionItem;
        } else if (typeStr == "G") {
            type = MarkMailAsImportant;
        }

        if (mInterceptedActions.contains(type) && checkIntercept) {
            return;
        }

        auto command = new MarkAsCommand(targetStatus, items, invert, mParent);
        command->execute();
    }

    void slotMarkAs()
    {
        const Akonadi::Item::List items = mGenericManager->selectedItems();
        if (items.isEmpty()) {
            return;
        }
        const QAction *action = qobject_cast<QAction *>(mParent->sender());
        Q_ASSERT(action);

        const QByteArray typeStr = action->data().toByteArray();

        markItemsAs(typeStr, items, true);
    }

    void markAllItemsAs(QByteArray typeStr, const Akonadi::Collection::List &collections, bool checkIntercept = true)
    {
        qCDebug(AKONADIMIME_LOG) << "Mark all as: " << typeStr;

        Akonadi::MessageStatus targetStatus;
        targetStatus.setStatusFromStr(QLatin1String(typeStr));

        bool invert = false;
        if (typeStr.startsWith('!')) {
            invert = true;
            typeStr.remove(0, 1);
        }
        bool recursive = false;
        if (typeStr.startsWith(':')) {
            recursive = true;
            typeStr.remove(0, 1);
        }

        StandardMailActionManager::Type type = MarkAllMailAsRead;
        if (typeStr == "U") {
            type = MarkAllMailAsUnread;
            targetStatus.setRead(true);
            invert = true;
        } else if (typeStr == "K") {
            type = MarkAllMailAsActionItem;
        } else if (typeStr == "G") {
            type = MarkAllMailAsImportant;
        }

        if (mInterceptedActions.contains(type) && checkIntercept) {
            return;
        }

        auto command = new MarkAsCommand(targetStatus, collections, invert, recursive, mParent);
        command->execute();
    }

    void slotMarkAllAs()
    {
        const Akonadi::Collection::List collections = mGenericManager->selectedCollections();
        if (collections.isEmpty()) {
            return;
        }

        const QAction *action = qobject_cast<QAction *>(mParent->sender());
        Q_ASSERT(action);
        const QByteArray typeStr = action->data().toByteArray();
        markAllItemsAs(typeStr, collections, true);
    }

    void slotMoveToTrash()
    {
        if (mInterceptedActions.contains(StandardMailActionManager::MoveToTrash)) {
            return;
        }

        if (mCollectionSelectionModel->selection().indexes().isEmpty()) {
            return;
        }

        const Item::List items = mGenericManager->selectedItems();
        if (items.isEmpty()) {
            return;
        }

        auto command = new MoveToTrashCommand(mCollectionSelectionModel->model(), items, mParent);
        command->execute();
    }

    void slotMoveAllToTrash()
    {
        if (mInterceptedActions.contains(StandardMailActionManager::MoveAllToTrash)) {
            return;
        }

        if (mCollectionSelectionModel->selection().indexes().isEmpty()) {
            return;
        }

        const Collection::List collections = mGenericManager->selectedCollections();
        if (collections.isEmpty()) {
            return;
        }

        auto command = new MoveToTrashCommand(mCollectionSelectionModel->model(), collections, mParent);
        command->execute();
    }

    void slotRemoveDuplicates()
    {
        if (mInterceptedActions.contains(StandardMailActionManager::RemoveDuplicates)) {
            return;
        }

        const Collection::List collections = mGenericManager->selectedCollections();
        if (collections.isEmpty()) {
            return;
        }

        auto job = new RemoveDuplicatesJob(collections, mParent);
        connect(job, &RemoveDuplicatesJob::finished, mParent, [this](KJob *job) {
            slotJobFinished(job);
        });
    }

    void slotJobFinished(KJob *job)
    {
        if (job->error()) {
            Util::showJobError(job);
        }
    }

    void slotEmptyAllTrash()
    {
        if (mInterceptedActions.contains(StandardMailActionManager::EmptyAllTrash)) {
            return;
        }

        auto command = new EmptyTrashCommand(const_cast<QAbstractItemModel *>(mCollectionSelectionModel->model()), mParent);
        command->execute();
    }

    void slotEmptyTrash()
    {
        if (mInterceptedActions.contains(StandardMailActionManager::EmptyTrash)) {
            return;
        }

        if (mCollectionSelectionModel->selection().indexes().isEmpty()) {
            return;
        }

        const Collection::List collections = mGenericManager->selectedCollections();
        if (collections.count() != 1) {
            return;
        }

        auto command = new EmptyTrashCommand(collections.first(), mParent);
        command->execute();
    }

    KActionCollection *const mActionCollection;
    QWidget *const mParentWidget;
    StandardActionManager *mGenericManager = nullptr;
    QItemSelectionModel *mCollectionSelectionModel = nullptr;
    QItemSelectionModel *mItemSelectionModel = nullptr;
    QHash<StandardMailActionManager::Type, QAction *> mActions;
    QSet<StandardMailActionManager::Type> mInterceptedActions;
    StandardMailActionManager *const mParent;
};

StandardMailActionManager::StandardMailActionManager(KActionCollection *actionCollection, QWidget *parent)
    : QObject(parent)
    , d(new Private(actionCollection, parent, this))
{
}

StandardMailActionManager::~StandardMailActionManager()
{
    delete d;
}

void StandardMailActionManager::setCollectionSelectionModel(QItemSelectionModel *selectionModel)
{
    d->mCollectionSelectionModel = selectionModel;
    d->mGenericManager->setCollectionSelectionModel(selectionModel);

    connect(selectionModel->model(), &QAbstractItemModel::rowsInserted, this, [this]() {
        d->updateActions();
    });
    connect(selectionModel->model(), &QAbstractItemModel::rowsRemoved, this, [this]() {
        d->updateActions();
    });
    // connecting to QItemSelectionModel::selectionChanged is done by mGenericManager

    d->updateActions();
}

void StandardMailActionManager::setItems(const Item::List &selectedItems)
{
    d->updateActionsFromSelectedItems(selectedItems);
}

void StandardMailActionManager::setItemSelectionModel(QItemSelectionModel *selectionModel)
{
    d->mItemSelectionModel = selectionModel;
    d->mGenericManager->setItemSelectionModel(selectionModel);

    // connecting to QItemSelectionModel::selectionChanged is done by mGenericManager

    // to catch item modifications, listen to the model's dataChanged signal as well
    connect(selectionModel->model(), &QAbstractItemModel::dataChanged, this, [this]() {
        d->updateActions();
    });

    d->updateActions();
}

QAction *StandardMailActionManager::createAction(Type type)
{
    if (d->mActions.contains(type)) {
        return d->mActions.value(type);
    }

    QAction *action = nullptr;

    switch (type) {
    case MarkMailAsRead:
        action = new QAction(d->mParentWidget);
        action->setIcon(QIcon::fromTheme(QStringLiteral("mail-mark-read")));
        action->setText(i18n("&Mark Message as Read"));
        action->setIconText(i18n("Mark as Read"));
        d->setHelpText(action, i18n("Mark selected messages as read."));
        action->setWhatsThis(i18n("Mark selected messages as read."));
        d->mActions.insert(MarkMailAsRead, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_mark_as_read"), action);
        action->setData(QByteArray("R"));
        d->mActionCollection->setDefaultShortcut(action, QKeySequence(Qt::CTRL | Qt::Key_R));
        connect(action, &QAction::triggered, this, [this]() {
            d->slotMarkAs();
        });
        break;
    case MarkMailAsUnread:
        action = new QAction(d->mParentWidget);
        action->setIcon(QIcon::fromTheme(QStringLiteral("mail-mark-unread")));
        action->setText(i18n("&Mark Message as Unread"));
        action->setIconText(i18n("Mark as Unread"));
        d->setHelpText(action, i18n("Mark selected messages as unread."));
        action->setWhatsThis(i18n("Mark selected messages as unread."));
        d->mActions.insert(MarkMailAsUnread, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_mark_as_unread"), action);
        d->mActionCollection->setDefaultShortcut(action, QKeySequence(Qt::CTRL | Qt::Key_U));
        action->setData(QByteArray("U"));
        connect(action, &QAction::triggered, this, [this]() {
            d->slotMarkAs();
        });
        break;
    case MarkMailAsImportant:
        action = new QAction(d->mParentWidget);
        action->setIcon(QIcon::fromTheme(QStringLiteral("mail-mark-important")));
        action->setText(i18n("&Mark Message as Important"));
        action->setIconText(i18n("Mark as Important"));
        d->setHelpText(action, i18n("Mark selected messages as important."));

        d->mActions.insert(MarkMailAsImportant, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_mark_as_important"), action);
        action->setData(QByteArray("G"));
        connect(action, &QAction::triggered, this, [this]() {
            d->slotMarkAs();
        });
        break;
    case MarkMailAsActionItem:
        action = new QAction(d->mParentWidget);
        action->setIcon(QIcon::fromTheme(QStringLiteral("mail-mark-task")));
        action->setText(i18n("&Mark Message as Action Item"));
        action->setIconText(i18n("Mark as Action Item"));
        d->setHelpText(action, i18n("Mark selected messages as action items."));
        action->setWhatsThis(i18n("Mark selected messages as action items."));
        d->mActions.insert(MarkMailAsActionItem, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_mark_as_action_item"), action);
        action->setData(QByteArray("K"));
        connect(action, &QAction::triggered, this, [this]() {
            d->slotMarkAs();
        });
        break;
    case MarkAllMailAsRead:
        action = new QAction(d->mParentWidget);
        action->setIcon(QIcon::fromTheme(QStringLiteral("mail-mark-read")));
        action->setText(i18n("Mark &All Messages as Read"));
        action->setIconText(i18n("Mark All as Read"));
        d->setHelpText(action, i18n("Mark all messages as read."));
        action->setWhatsThis(i18n("Mark all messages as read."));
        d->mActions.insert(MarkAllMailAsRead, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_mark_all_as_read"), action);
        action->setData(QByteArray("R"));
        connect(action, &QAction::triggered, this, [this]() {
            d->slotMarkAllAs();
        });
        break;
    case MarkAllMailAsReadRecursive:
        action = new QAction(d->mParentWidget);
        action->setIcon(QIcon::fromTheme(QStringLiteral("mail-mark-read")));
        action->setText(i18n("Mark &All Messages as Read Recursively"));
        action->setIconText(i18n("Mark All as Read Recursively"));
        d->setHelpText(action, i18n("Mark all messages as read recursively."));
        action->setWhatsThis(i18n("Mark all messages as read recursively."));
        d->mActions.insert(MarkAllMailAsReadRecursive, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_mark_all_as_read_recursive"), action);
        action->setData(QByteArray(":R"));
        connect(action, &QAction::triggered, this, [this]() {
            d->slotMarkAllAs();
        });
        break;
    case MarkAllMailAsUnread:
        action = new QAction(d->mParentWidget);
        action->setIcon(QIcon::fromTheme(QStringLiteral("mail-mark-unread")));
        action->setText(i18n("Mark &All Messages as Unread"));
        action->setIconText(i18n("Mark All as Unread"));
        d->setHelpText(action, i18n("Mark all messages as unread."));
        action->setWhatsThis(i18n("Mark all messages as unread."));
        d->mActions.insert(MarkAllMailAsUnread, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_mark_all_as_unread"), action);
        action->setData(QByteArray("U"));
        connect(action, &QAction::triggered, this, [this]() {
            d->slotMarkAllAs();
        });
        break;
    case MarkAllMailAsImportant:
        action = new QAction(d->mParentWidget);
        action->setIcon(QIcon::fromTheme(QStringLiteral("mail-mark-important")));
        action->setText(i18n("Mark &All Messages as Important"));
        action->setIconText(i18n("Mark All as Important"));
        d->setHelpText(action, i18n("Mark all messages as important."));
        action->setWhatsThis(i18n("Mark all messages as important."));
        d->mActions.insert(MarkAllMailAsImportant, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_mark_all_as_important"), action);
        action->setData(QByteArray("G"));
        connect(action, &QAction::triggered, this, [this]() {
            d->slotMarkAllAs();
        });
        break;
    case MarkAllMailAsActionItem:
        action = new QAction(d->mParentWidget);
        action->setIconText(i18n("Mark All as Action Item"));
        action->setText(i18n("Mark &All Messages as Action Item"));
        action->setIcon(QIcon::fromTheme(QStringLiteral("mail-mark-task")));
        d->setHelpText(action, i18n("Mark all messages as action items."));
        action->setWhatsThis(i18n("Mark all messages as action items."));
        d->mActions.insert(MarkAllMailAsActionItem, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_mark_all_as_action_item"), action);
        action->setData(QByteArray("K"));
        connect(action, &QAction::triggered, this, [this]() {
            d->slotMarkAllAs();
        });
        break;
    case MoveToTrash:
        action = new QAction(d->mParentWidget);
        action->setIcon(QIcon::fromTheme(QStringLiteral("user-trash")));
        action->setText(i18n("Move to &Trash"));
        d->mActionCollection->setDefaultShortcut(action, QKeySequence(Qt::Key_Delete));
        d->setHelpText(action, i18n("Move selected messages to the trash folder."));
        action->setWhatsThis(i18n("Move selected messages to the trash folder."));
        d->mActions.insert(MoveToTrash, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_move_to_trash"), action);
        connect(action, &QAction::triggered, this, [this]() {
            d->slotMoveToTrash();
        });
        break;
    case MoveAllToTrash:
        action = new QAction(d->mParentWidget);
        action->setIcon(QIcon::fromTheme(QStringLiteral("user-trash")));
        action->setText(i18n("Move All to &Trash"));
        d->setHelpText(action, i18n("Move all messages to the trash folder."));
        action->setWhatsThis(i18n("Move all messages to the trash folder."));
        d->mActions.insert(MoveAllToTrash, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_move_all_to_trash"), action);
        connect(action, &QAction::triggered, this, [this]() {
            d->slotMoveAllToTrash();
        });
        break;
    case RemoveDuplicates:
        action = new QAction(d->mParentWidget);
        action->setText(i18n("Remove &Duplicate Messages"));
        d->setHelpText(action, i18n("Remove duplicate messages."));
        action->setWhatsThis(i18n("Remove duplicate messages."));
        d->mActionCollection->setDefaultShortcut(action, QKeySequence(Qt::CTRL | Qt::Key_Asterisk));
        d->mActions.insert(RemoveDuplicates, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_remove_duplicates"), action);
        connect(action, &QAction::triggered, this, [this]() {
            d->slotRemoveDuplicates();
        });
        break;
    case EmptyAllTrash:
        action = new QAction(d->mParentWidget);
        action->setText(i18n("Empty All &Trash Folders"));
        d->setHelpText(action, i18n("Permanently delete all messages from all trash folders."));
        action->setWhatsThis(i18n("Permanently delete all messages from all trash folders."));
        d->mActions.insert(EmptyAllTrash, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_empty_all_trash"), action);
        connect(action, &QAction::triggered, this, [this]() {
            d->slotEmptyAllTrash();
        });
        break;
    case EmptyTrash:
        action = new QAction(d->mParentWidget);
        action->setText(i18n("E&mpty Trash"));
        d->setHelpText(action, i18n("Permanently delete all messages from the trash folder."));
        action->setWhatsThis(i18n("Permanently delete all messages from the trash folder."));
        d->mActions.insert(EmptyTrash, action);
        d->mActionCollection->addAction(QStringLiteral("akonadi_empty_trash"), action);
        connect(action, &QAction::triggered, this, [this]() {
            d->slotEmptyTrash();
        });
        break;
    default:
        Q_ASSERT(false); // should never happen
        break;
    }

    return action;
}

QAction *StandardMailActionManager::createAction(StandardActionManager::Type type)
{
    QAction *act = d->mGenericManager->action(type);
    if (!act) {
        act = d->mGenericManager->createAction(type);
    }
    d->updateGenericAction(type);
    return act;
}

void StandardMailActionManager::createAllActions()
{
    createAction(MarkMailAsRead);
    createAction(MarkMailAsUnread);
    createAction(MarkMailAsImportant);
    createAction(MarkMailAsActionItem);
    createAction(MarkAllMailAsRead);
    createAction(MarkAllMailAsReadRecursive);
    createAction(MarkAllMailAsUnread);
    createAction(MarkAllMailAsImportant);
    createAction(MarkAllMailAsActionItem);
    createAction(MoveToTrash);
    createAction(MoveAllToTrash);
    createAction(RemoveDuplicates);
    createAction(EmptyAllTrash);
    createAction(EmptyTrash);

    d->mGenericManager->createAllActions();
    d->updateGenericAllActions();

    d->updateActions();
}

QAction *StandardMailActionManager::action(Type type) const
{
    return d->mActions.value(type);
}

QAction *StandardMailActionManager::action(StandardActionManager::Type type) const
{
    return d->mGenericManager->action(type);
}

void StandardMailActionManager::setActionText(StandardActionManager::Type type, const KLocalizedString &text)
{
    d->mGenericManager->setActionText(type, text);
}

void StandardMailActionManager::interceptAction(Type type, bool intercept)
{
    if (intercept) {
        d->mInterceptedActions.insert(type);
    } else {
        d->mInterceptedActions.remove(type);
    }
}

void StandardMailActionManager::interceptAction(StandardActionManager::Type type, bool intercept)
{
    d->mGenericManager->interceptAction(type, intercept);
}

Akonadi::Collection::List StandardMailActionManager::selectedCollections() const
{
    return d->mGenericManager->selectedCollections();
}

Akonadi::Item::List StandardMailActionManager::selectedItems() const
{
    return d->mGenericManager->selectedItems();
}

void StandardMailActionManager::setFavoriteCollectionsModel(FavoriteCollectionsModel *favoritesModel)
{
    d->mGenericManager->setFavoriteCollectionsModel(favoritesModel);
}

void StandardMailActionManager::setFavoriteSelectionModel(QItemSelectionModel *selectionModel)
{
    d->mGenericManager->setFavoriteSelectionModel(selectionModel);
}

void StandardMailActionManager::setCollectionPropertiesPageNames(const QStringList &names)
{
    d->mGenericManager->setCollectionPropertiesPageNames(names);
}

Akonadi::StandardActionManager *StandardMailActionManager::standardActionManager() const
{
    return d->mGenericManager;
}

void StandardMailActionManager::markItemsAs(const QByteArray &typeStr, const Item::List &items, bool checkIntercept)
{
    d->markItemsAs(typeStr, items, checkIntercept);
}

void StandardMailActionManager::markAllItemsAs(const QByteArray &typeStr, const Collection::List &collections, bool checkIntercept)
{
    d->markAllItemsAs(typeStr, collections, checkIntercept);
}

#include "moc_standardmailactionmanager.cpp"

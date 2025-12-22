/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"

#include <Akonadi/AgentInstance>

// krazy:excludeall=dpointer

namespace Akonadi
{
/*!
  \brief An interface for applications to interact with the dispatcher agent.

  This class provides methods such as send queued messages (\sa
  dispatchManually) and retry sending (\sa retryDispatching).

  This class also takes care of registering the attributes that the mail
  dispatcher agent and MailTransport use.

  @author Constantin Berzan <exit3219@gmail.com>
  \since 4.4
*/
class AKONADI_MIME_EXPORT DispatcherInterface
{
public:
    /*!
      Creates a new dispatcher interface.
    */
    DispatcherInterface();

    /*!
      Returns the current instance of the mail dispatcher agent. May return an invalid
      AgentInstance in case it cannot find the mail dispatcher agent.
    */
    [[nodiscard]] Akonadi::AgentInstance dispatcherInstance() const;

    /*!
      Looks for messages in the outbox with DispatchMode::Manual and marks them
      DispatchMode::Automatic for sending.
    */
    void dispatchManually();

    /*!
      Looks for messages in the outbox with ErrorAttribute, and clears them and
      queues them again for sending.
    */
    void retryDispatching();

    /*!
      Looks for messages in the outbox with DispatchMode::Manual and changes the
      Transport for them to the one with id \a transportId.

      \a transportId the transport to dispatch "manual dispatch" messages with
      \since 4.5
    */
    void dispatchManualTransport(int transportId);
};
} // namespace MailTransport

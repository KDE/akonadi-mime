# Akonadi Mime

Akonadi Mime is a library that effectively bridges the type-agnostic API of
the Akonadi client libraries and the domain-specific KMime library. It provides
jobs, models and other helpers to make working with emails through Akonadi easier.

The most notable classes are "commands" to update email flags (like read, spam,
etc.) or move it to trash and Akonadi::StandardMailActionManager which provides
set of email-specific QActions.

/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef COMMANDBASE_H
#define COMMANDBASE_H

#include <QObject>
#include "akonadi-mime_export.h"
namespace Akonadi {
class AKONADI_MIME_EXPORT CommandBase : public QObject
{
    Q_OBJECT

public:
    explicit CommandBase(QObject *parent = nullptr);
    virtual void execute() = 0;

    enum Result {
        Undefined,
        OK,
        Canceled,
        Failed
    };

Q_SIGNALS:
    void result(Akonadi::CommandBase::Result);

protected Q_SLOTS:
    virtual void emitResult(Akonadi::CommandBase::Result result);
};
}
#endif // COMMANDBASE_H

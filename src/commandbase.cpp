/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company, <info@kdab.com>
    SPDX-FileCopyrightText: 2010 Andras Mantia <andras@kdab.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "commandbase.h"
using namespace Akonadi;
CommandBase::CommandBase(QObject *parent)
    : QObject(parent)
{
}

void CommandBase::emitResult(Result value)
{
    Q_EMIT result(value);
    deleteLater();
}

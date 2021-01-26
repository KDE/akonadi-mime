/*
    SPDX-FileCopyrightText: 2009 Igor Trindade Oliveira <igor_trindade@yahoo.com.br>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "testmaildir.h"
#include "maildir/maildir20percentread.h"
#include "maildir/maildirfetchallheaders.h"
#include "maildir/maildirfetchunreadheaders.h"
#include "maildir/maildirimport.h"

TestMailDir::TestMailDir(const QString &dir)
{
    addTest(new MailDirImport(dir));
    addTest(new MailDirFetchAllHeaders());
    addTest(new MailDir20PercentAsRead());
    addTest(new MailDirFetchUnreadHeaders());
}

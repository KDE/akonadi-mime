# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
kde_enable_exceptions()

set(benchmarker_SRCS
  maildir/maildir.cpp  
  maildir/maildirimport.cpp  
  main.cpp  
  maketest.cpp  
  test.cpp  
  testmaildir.cpp
  maildir/maildirfetchallheaders.cpp
  maildir/maildir20percentread.cpp
  maildir/maildirfetchunreadheaders.cpp
  maildir/maildirremovereadmessages.cpp

  testvcard.cpp
  vcard/vcard.cpp
  vcard/vcardimport.cpp
)

add_executable(akonadi_benchmarker ${benchmarker_SRCS})

target_link_libraries(akonadi_benchmarker
  KPim6::AkonadiMime
  KPim6::Mime
  Qt::Test
  KPim6::AkonadiCore
  KF6::I18n
  Qt::Widgets
  Qt::DBus
)

install(TARGETS akonadi_benchmarker ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

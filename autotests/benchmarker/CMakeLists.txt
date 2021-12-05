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
  KF5::AkonadiMime
  KF5::Mime
  Qt::Test
  KF5::AkonadiCore
  KF5::I18n
  Qt::Widgets
  Qt::DBus
)

install(TARGETS akonadi_benchmarker ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

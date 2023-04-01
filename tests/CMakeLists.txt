# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
find_package(Qt${QT_MAJOR_VERSION} ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Test Widgets)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

add_executable( requester foldersrequester.cpp foldersrequester.h )
target_link_libraries( requester KPim${KF_MAJOR_VERSION}::AkonadiMime KF${KF_MAJOR_VERSION}::I18n Qt::Widgets )

# convenience macro to add akonadi demo application
macro(add_akonadimime_demo _source)
  set(_test ${_source})
  get_filename_component(_name ${_source} NAME_WE)
  add_executable(${_name} ${_test})
  target_link_libraries(${_name}  KPim${KF_MAJOR_VERSION}AkonadiMime KPim${KF_MAJOR_VERSION}::Mime KPim${KF_MAJOR_VERSION}::AkonadiCore KF${KF_MAJOR_VERSION}::I18n Qt::Widgets)
endmacro()

# demo applications
add_akonadimime_demo(headfetcher.cpp)


#Laurent : Look at how to readd it => depend against kmailtransport
#find_package(KF${KF_MAJOR_VERSION}TextWidgets ${KF_MIN_VERSION} CONFIG REQUIRED)

#set(queuer_srcs queuer.cpp queuer.h)
#add_executable(queuer ${queuer_srcs})
#ecm_mark_as_test(queuer)
#target_link_libraries(queuer KPim${KF_MAJOR_VERSION}::AkonadiMime Qt::Widgets KF${KF_MAJOR_VERSION}::I18n KF${KF_MAJOR_VERSION}::ConfigGui KF${KF_MAJOR_VERSION}::TextWidgets)

set( sendqueued_srcs sendqueued.cpp sendqueued.h)
add_executable( sendqueued ${sendqueued_srcs} )
ecm_mark_as_test(sendqueued)
target_link_libraries( sendqueued KPim${KF_MAJOR_VERSION}::AkonadiMime Qt::Widgets)

set( clearerror_srcs clearerror.cpp clearerror.h)
add_executable( clearerror ${clearerror_srcs} )
ecm_mark_as_test(clearerror)
target_link_libraries( clearerror KPim${KF_MAJOR_VERSION}::AkonadiMime Qt::Widgets)

set( abort_srcs abort.cpp abort.h)
add_executable( abort ${abort_srcs} )
ecm_mark_as_test(abort)
target_link_libraries( abort KPim${KF_MAJOR_VERSION}::AkonadiMime Qt::Widgets)

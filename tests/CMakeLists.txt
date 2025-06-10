# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Test Widgets)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

add_executable(requester foldersrequester.cpp foldersrequester.h)
target_link_libraries(requester KPim6::AkonadiMime KF6::I18n Qt::Widgets)

# convenience macro to add akonadi demo application
macro(add_akonadimime_demo _source)
    set(_test ${_source})
    get_filename_component(_name ${_source} NAME_WE)
    add_executable(${_name} ${_test})
    target_link_libraries(
        ${_name}
        KPim6AkonadiMime
        KPim6::Mime
        KPim6::AkonadiCore
        KF6::I18n
        Qt::Widgets
    )
endmacro()

# demo applications
add_akonadimime_demo(headfetcher.cpp)

#Laurent : Look at how to readd it => depend against kmailtransport
#find_package(KF6TextWidgets ${KF_MIN_VERSION} CONFIG REQUIRED)

#set(queuer_srcs queuer.cpp queuer.h)
#add_executable(queuer ${queuer_srcs})
#ecm_mark_as_test(queuer)
#target_link_libraries(queuer KPim6::AkonadiMime Qt::Widgets KF6::I18n KF6::ConfigGui KF6::TextWidgets)

set(sendqueued_srcs sendqueued.cpp sendqueued.h)
add_executable(sendqueued ${sendqueued_srcs})
ecm_mark_as_test(sendqueued)
target_link_libraries(sendqueued KPim6::AkonadiMime Qt::Widgets)

set(clearerror_srcs clearerror.cpp clearerror.h)
add_executable(clearerror ${clearerror_srcs})
ecm_mark_as_test(clearerror)
target_link_libraries(clearerror KPim6::AkonadiMime Qt::Widgets)

set(abort_srcs abort.cpp abort.h)
add_executable(abort ${abort_srcs})
ecm_mark_as_test(abort)
target_link_libraries(abort KPim6::AkonadiMime Qt::Widgets)

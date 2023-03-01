# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
include(ECMMarkAsTest)

macro(ADD_AKONADIMIME_TEST)
  foreach(_testName ${ARGN})
    add_executable(${_testName} ${_testName}.cpp)
    add_test(NAME akonadimime-${_testName} COMMAND ${_testName})
    target_link_libraries(${_testName}
      KPim${KF_MAJOR_VERSION}::Mime
      Qt::Test
      KPim${KF_MAJOR_VERSION}::AkonadiCore
      KPim${KF_MAJOR_VERSION}::AkonadiMime
    )
    set_target_properties(${_testName} PROPERTIES COMPILE_FLAGS -DTEST_DATA_DIR="\\"${CMAKE_CURRENT_SOURCE_DIR}/\\"" )
    ecm_mark_as_test(${_testName})
  endforeach()
endmacro()

add_akonadimime_test(
  messagetest
)

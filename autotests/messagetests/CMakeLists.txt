include(ECMMarkAsTest)

macro(ADD_AKONADIMIME_TEST)
  foreach(_testName ${ARGN})
    add_executable(${_testName} ${_testName}.cpp)
    add_test(NAME akonadimime-${_testName} COMMAND ${_testName})
    target_link_libraries(${_testName}
      KF5::Mime
      Qt::Test
      KF5::AkonadiCore
      KF5::AkonadiMime
    )
    set_target_properties(${_testName} PROPERTIES COMPILE_FLAGS -DTEST_DATA_DIR="\\"${CMAKE_CURRENT_SOURCE_DIR}/\\"" )
    ecm_mark_as_test(${_testName})
  endforeach()
endmacro()

add_akonadimime_test(
  messagetest
)

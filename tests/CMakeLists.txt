find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Test Widgets)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( requester_srcs foldersrequester.cpp )
add_executable( requester "${requester_srcs}" )
target_link_libraries( requester KF5::AkonadiMime KF5::I18n Qt::Widgets )

# convenience macro to add akonadi demo application
macro(add_akonadimime_demo _source)
  set(_test ${_source})
  get_filename_component(_name ${_source} NAME_WE)
  add_executable(${_name} ${_test})
  target_link_libraries(${_name}  KF5AkonadiMime KF5::Mime KF5::AkonadiCore KF5::I18n Qt::Widgets)
endmacro()

# demo applications
add_akonadimime_demo(headfetcher.cpp)

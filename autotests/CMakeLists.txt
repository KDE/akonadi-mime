include(ECMAddTests)

find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Test Widgets)

add_subdirectory(messagetests)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(
    ${CMAKE_SOURCE_DIR}/serializers
    ${CMAKE_BINARY_DIR}/serializers
)

# for racetest
set(requester_exe_string "QLatin1String( \\\"${CMAKE_CURRENT_BINARY_DIR}/requester\\\" )" )
add_definitions( -DREQUESTER_EXE="${requester_exe_string}" )

#macro(add_akonadi_isolated_test _source)
#  kconfig_add_kcfg_files( _srcList ../src/specialmailcollectionssettings.kcfgc )

### TODO: add the necessary API in SpecialCollections
#add_akonadi_isolated_test(SOURCE localfolderstest.cpp LINK_LIBRARIES "KF5::ConfigGui;KF5::AkonadiMime;KF5::I18n")
#add_akonadi_isolated_test(SOURCE localfoldersrequestjobtest.cpp LINK_LIBRARIES "KF5::ConfigGui;KF5::AkonadiMime;KF5::I18n")

# TODO: debug and fix this test
#add_akonadi_isolated_test(racetest.cpp)

add_subdirectory(benchmarker)

ecm_add_test(mailserializerplugintest.cpp ${CMAKE_BINARY_DIR}/serializers/akonadi_serializer_mail_debug.cpp
    LINK_LIBRARIES KF5::AkonadiMime KF5::AkonadiCore KF5::AkonadiPrivate KF5::Mime Qt5::Test
    TEST_NAME mailserializerplugintest
)
ecm_add_test(mailserializertest.cpp
    ${CMAKE_SOURCE_DIR}/serializers/akonadi_serializer_mail.cpp
    ${CMAKE_BINARY_DIR}/serializers/akonadi_serializer_mail_debug.cpp
    LINK_LIBRARIES KF5::AkonadiMime KF5::AkonadiCore KF5::AkonadiPrivate KF5::Mime Qt5::Test
    TEST_NAME mailserializertest
)
ecm_add_test(newmailnotifierattributetest.cpp
    LINK_LIBRARIES KF5::AkonadiMime KF5::AkonadiCore Qt5::Test
    TEST_NAME newmailnotifierattributetest
)
ecm_add_test(pop3resourceattributetest.cpp
    LINK_LIBRARIES KF5::AkonadiMime KF5::AkonadiCore Qt5::Test
    TEST_NAME pop3resourceattributetest
)

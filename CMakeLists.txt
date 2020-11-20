cmake_minimum_required(VERSION 3.5)

set(PIM_VERSION "5.16.40")
project(Akonadi-Mime VERSION ${PIM_VERSION})

# ECM setup
set(KF5_MIN_VERSION "5.76.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(QT_REQUIRED_VERSION "5.13.0")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(ECMQtDeclareLoggingCategory)

set(AKONADI_MIME_VERSION ${PIM_VERSION})
set(AKONADI_VERSION "5.16.40")
set(KF5_MIME_VERSION "5.16.40")

ecm_setup_version(PROJECT VARIABLE_PREFIX AKONADIMIME
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/akonadi-mime_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5AkonadiMimeConfigVersion.cmake"
    SOVERSION 5
    )

########### Find packages ###########
find_package(KF5Codecs ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5KIO ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5XmlGui ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Mime ${KF5_MIME_VERSION} CONFIG REQUIRED)
find_package(KF5ItemModels ${KF5_MIN_VERSION} CONFIG REQUIRED)

find_package(KF5Akonadi ${AKONADI_VERSION} CONFIG REQUIRED)

set( SharedMimeInfo_MINIMUM_VERSION "1.3" )
find_package(SharedMimeInfo ${SharedMimeInfo_MINIMUM_VERSION} REQUIRED)

find_package(LibXslt)
set_package_properties(LibXslt PROPERTIES
    DESCRIPTION "xsltproc"
    URL "http://xmlsoft.org/XSLT/"
    TYPE REQUIRED
    PURPOSE "Needed to generate D-Bus interface specifications"
    )

########### Targets ###########

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f00)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x054D00)

add_definitions(-DQT_NO_FOREACH)
add_definitions(-DQT_NO_KEYWORDS)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(BUILD_TESTING)
    set(AKONADI_MIME_TEST_EXPORT AKONADI_MIME_EXPORT)
endif()
configure_file(akonadi-mimeprivate_export.h.in "${CMAKE_CURRENT_BINARY_DIR}/akonadi-mimeprivate_export.h")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(serializers)

if(BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5AkonadiMime")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5AkonadiMimeConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5AkonadiMimeConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5AkonadiMimeConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5AkonadiMimeConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
    )

install(EXPORT KF5AkonadiMimeTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5AkonadiMimeTargets.cmake NAMESPACE KF5::)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/akonadi-mime_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel
    )

ecm_qt_install_logging_categories(
        EXPORT AKONADIMIME
        FILE akonadi-mime.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
        )


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

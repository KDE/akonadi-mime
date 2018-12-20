cmake_minimum_required(VERSION 3.5)

set(PIM_VERSION "5.10.40")
project(Akonadi-Mime VERSION ${PIM_VERSION})

# ECM setup
set(KF5_MIN_VERSION "5.53.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(QT_REQUIRED_VERSION "5.10.0")
include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)



set(AKONADI_MIME_VERSION ${PIM_VERSION})
set(AKONADI_VERSION "5.10.40")
set(KF5_MIME_VERSION "5.10.40")

ecm_setup_version(PROJECT VARIABLE_PREFIX AKONADIMIME
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/akonadi-mime_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5AkonadiMimeConfigVersion.cmake"
    SOVERSION 5
    )

########### Find packages ###########
find_package(KF5DBusAddons ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5KIO ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Config ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5XmlGui ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5Mime ${KF5_MIME_VERSION} CONFIG REQUIRED)
find_package(KF5ItemModels ${KF5_MIME_VERSION} CONFIG REQUIRED)

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

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

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

install(FILES akonadi-mime.categories DESTINATION ${KDE_INSTALL_CONFDIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

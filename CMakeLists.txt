cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(PIM_VERSION "5.18.0")
project(Akonadi-Mime VERSION ${PIM_VERSION})

# ECM setup
set(KF5_MIN_VERSION "5.83.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(QT_REQUIRED_VERSION "5.15.0")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(KDEGitCommitHooks)
include(ECMQtDeclareLoggingCategory)

set(AKONADI_MIME_VERSION ${PIM_VERSION})
set(AKONADI_VERSION "5.17.90")
set(KF5_MIME_VERSION "5.17.90")

ecm_setup_version(PROJECT VARIABLE_PREFIX AKONADIMIME
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/akonadi-mime_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5AkonadiMimeConfigVersion.cmake"
    SOVERSION 5
    )

########### Find packages ###########
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
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055400)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()
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


kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

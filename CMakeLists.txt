# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(PIM_VERSION "6.4.40")
project(Akonadi-Mime VERSION ${PIM_VERSION})

# ECM setup
set(KF_MIN_VERSION "6.11.0")

find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(QT_REQUIRED_VERSION "6.8.0")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)

include(ECMSetupVersion)
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES
src/*.cpp
src/*.h
autotests/*.cpp
autotests/*.h
serializers/*.cpp
serializers/*.h
tests/*.cpp
tests/*.h
)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(ECMFeatureSummary)
include(ECMCheckOutboundLicense)
ecm_check_outbound_license(LICENSES GPL-2.0-only  FILES ${ALL_CLANG_FORMAT_SOURCE_FILES})

include(ECMAddQch)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

set(AKONADI_MIME_VERSION ${PIM_VERSION})
set(AKONADI_VERSION "6.3.40")
set(KPIM_MIME_VERSION "6.3.41")

ecm_setup_version(PROJECT VARIABLE_PREFIX AKONADIMIME
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/akonadi-mime_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim6AkonadiMimeConfigVersion.cmake"
    SOVERSION 6
   )

########### Find packages ###########
find_package(KF6KIO ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6I18n ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6XmlGui ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KPim6Mime ${KPIM_MIME_VERSION} CONFIG REQUIRED)
find_package(KF6ItemModels ${KF_MIN_VERSION} CONFIG REQUIRED)

find_package(KPim6Akonadi ${AKONADI_VERSION} CONFIG REQUIRED)

set(SharedMimeInfo_MINIMUM_VERSION "1.10")
find_package(SharedMimeInfo ${SharedMimeInfo_MINIMUM_VERSION} REQUIRED)

find_package(LibXslt)
set_package_properties(LibXslt PROPERTIES
    DESCRIPTION "xsltproc"
    URL "http://xmlsoft.org/XSLT/"
    TYPE REQUIRED
    PURPOSE "Needed to generate D-Bus interface specifications"
   )

########### Targets ###########

add_definitions(-DQT_NO_CONTEXTLESS_CONNECT)
ecm_set_disabled_deprecation_versions(QT 6.9.0
     KF 6.12.0
)

option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if(USE_UNITY_CMAKE_SUPPORT)
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
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6AkonadiMime")
if(BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KPim6AkonadiMime_QCH
        FILE KPim6AkonadiMimeQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
   )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim6AkonadiMimeQchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimAkonadiMimeConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6AkonadiMimeConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
   )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6AkonadiMimeConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6AkonadiMimeConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
   )

install(EXPORT KPim6AkonadiMimeTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KPim6AkonadiMimeTargets.cmake NAMESPACE KPim6::)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/akonadi-mime_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/AkonadiMime COMPONENT Devel
   )

ecm_qt_install_logging_categories(
        EXPORT AKONADIMIME
        FILE akonadi-mime.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
       )


kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)

ecm_feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

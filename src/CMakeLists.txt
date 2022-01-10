# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_definitions(-DTRANSLATION_DOMAIN=\"libakonadi-kmime5\")
# generates a D-Bus interface description from a KConfigXT file
macro(kcfg_generate_dbus_interface _kcfg _name)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_name}.xml
        COMMAND ${LIBXSLT_XSLTPROC_EXECUTABLE} --stringparam interfaceName ${_name}
        ${KF5Akonadi_DATA_DIR}/kcfg2dbus.xsl
        ${_kcfg}
        > ${CMAKE_CURRENT_BINARY_DIR}/${_name}.xml
        ${_kcfg}
        )
endmacro()

macro(add_resource_iface _kcfgFile _ifaceName _className)
    kcfg_generate_dbus_interface(${CMAKE_CURRENT_SOURCE_DIR}/${_kcfgFile} ${_ifaceName})
    string(TOLOWER ${_className} _codeFile)
    set_source_files_properties( ${CMAKE_CURRENT_BINARY_DIR}/${_ifaceName}.xml PROPERTIES INCLUDE "metatype.h")
    qt_add_dbus_interface(akonadimime_SRCS
        ${CMAKE_CURRENT_BINARY_DIR}/${_ifaceName}.xml ${_codeFile} ${_className}
        )
endmacro()

########### next target ###############

set(akonadimime_SRCS
    addressattribute.cpp
    attributeregistrar.cpp
    removeduplicatesjob.cpp
    specialmailcollections.cpp
    specialmailcollectionsrequestjob.cpp
    specialmailcollectionsdiscoveryjob.cpp
    messagefolderattribute.cpp
    messagemodel.cpp
    messageparts.cpp
    messageflags.cpp
    messagestatus.cpp
    newmailnotifierattribute.cpp
    pop3resourceattribute.cpp

    commandbase.cpp
    util.cpp
    emptytrashcommand.cpp
    markascommand.cpp
    movecommand.cpp
    movetotrashcommand.cpp
    standardmailactionmanager.cpp

    messagestatus.h
    util_p.h
    emptytrashcommand.h
    specialmailcollectionsrequestjob.h
    movecommand.h
    newmailnotifierattribute.h
    specialmailcollectionstesting_p.h
    standardmailactionmanager.h
    messagefolderattribute.h
    removeduplicatesjob.h
    movetotrashcommand.h
    specialmailcollectionsdiscoveryjob.h
    messageflags.h
    pop3resourceattribute.h
    addressattribute.h
    markascommand.h
    messagemodel.h
    commandbase.h
    specialmailcollections.h
    metatype.h
    messageparts.h
    )

ecm_qt_declare_logging_category(akonadimime_SRCS HEADER akonadi_mime_debug.h IDENTIFIER AKONADIMIME_LOG CATEGORY_NAME log_akonadi_mime)

add_resource_iface(imapresource.kcfg
    org.kde.Akonadi.Imap.Settings ImapSettings)

# TODO: add the necessary API in SpecialCollections
#if (BUILD_TESTING)
#    list(APPEND akonadimime_SRCS specialmailcollectionstesting.cpp)
#endif()

kconfig_add_kcfg_files(akonadimime_SRCS
    specialmailcollectionssettings.kcfgc
    )

install(FILES
    specialmailcollections.kcfg
    DESTINATION ${KDE_INSTALL_KCFGDIR}
    )

add_library(KF5AkonadiMime ${akonadimime_SRCS})
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5AkonadiMime PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5AkonadiMime BASE_NAME akonadi-mime)

add_library(KF5::AkonadiMime ALIAS KF5AkonadiMime)

# NOTE: The include directories remain 'akonadi/kmime' to be as SC as possible.
target_include_directories(KF5AkonadiMime INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/Akonadi/KMime>")
target_include_directories(KF5AkonadiMime PUBLIC "$<BUILD_INTERFACE:${Akonadi-Mime_SOURCE_DIR}/src;${Akonadi-Mime_BINARY_DIR}/src>")
target_include_directories(KF5AkonadiMime INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/akonadi/kmime>")

target_link_libraries(KF5AkonadiMime
    PUBLIC
    KF5::AkonadiCore
    PRIVATE
    KF5::Mime
    KF5::AkonadiWidgets
    KF5::KIOCore
    KF5::I18n
    KF5::XmlGui
    )

set_target_properties(KF5AkonadiMime PROPERTIES
    VERSION ${AKONADIMIME_VERSION}
    SOVERSION ${AKONADIMIME_SOVERSION}
    EXPORT_NAME AkonadiMime
    )

install(TARGETS
    KF5AkonadiMime
    EXPORT KF5AkonadiMimeTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
    )


ecm_generate_headers(AkonadiMime_CamelCase_HEADERS
    HEADER_NAMES

    AddressAttribute
    CommandBase
    MessageFlags
    MessageFolderAttribute
    MessageModel
    MessageParts
    MessageStatus
    MarkAsCommand
    MoveCommand
    NewMailNotifierAttribute
    Pop3ResourceAttribute
    RemoveDuplicatesJob
    SpecialMailCollections
    SpecialMailCollectionsDiscoveryJob
    SpecialMailCollectionsRequestJob
    StandardMailActionManager
    REQUIRED_HEADERS AkonadiMime_HEADERS
    PREFIX Akonadi/KMime
    )

install(FILES
    ${AkonadiMime_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/Akonadi/KMime
    COMPONENT Devel
    )

install(FILES
    ${AkonadiMime_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/akonadi-mime_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/akonadi/kmime
    COMPONENT Devel
    )

ecm_generate_pri_file(BASE_NAME AkonadiMime
    LIB_NAME KF5AkonadiMime
    DEPS "AkonadiCore" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/Akonadi/KMime
    )

install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})


if (BUILD_QCH)
    ecm_add_qch(
        KF5AkonadiMime_QCH
        NAME KF5AkonadiMime
        BASE_NAME KF5AkonadiMime
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${AkonadiMime_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt5Core_QCH
            Qt5Gui_QCH
            Qt5Widgets_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            AKONADI_MIME_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

## install the MIME type spec file for KDEPIM specific MIME types
install(FILES x-vnd.kde.contactgroup.xml DESTINATION ${KDE_INSTALL_MIMEDIR})
update_xdg_mimetypes(${KDE_INSTALL_MIMEDIR})


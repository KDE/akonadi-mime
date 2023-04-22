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
    set_source_files_properties( ${CMAKE_CURRENT_BINARY_DIR}/${_ifaceName}.xml PROPERTIES INCLUDE "metatype.h")
    qt_add_dbus_interface(akonadimime_SRCS
        ${CMAKE_CURRENT_BINARY_DIR}/${_ifaceName}.xml ${_className}
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
    markascommandhelper_p.h
    markascommandhelper.cpp

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

    mdnstateattribute.h
    mdnstateattribute.cpp
    )


add_resource_iface(imapresource.kcfg
    org.kde.Akonadi.Imap.Settings imapsettings)

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

add_library(KPim${KF_MAJOR_VERSION}AkonadiMime ${akonadimime_SRCS})
ecm_qt_declare_logging_category(KPim${KF_MAJOR_VERSION}AkonadiMime HEADER akonadi_mime_debug.h IDENTIFIER AKONADIMIME_LOG CATEGORY_NAME log_akonadi_mime)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim${KF_MAJOR_VERSION}AkonadiMime PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KPim${KF_MAJOR_VERSION}AkonadiMime BASE_NAME akonadi-mime)

add_library(KPim${KF_MAJOR_VERSION}::AkonadiMime ALIAS KPim${KF_MAJOR_VERSION}AkonadiMime)
add_library(KF5::AkonadiMime ALIAS KPim${KF_MAJOR_VERSION}AkonadiMime)

target_include_directories(KPim${KF_MAJOR_VERSION}AkonadiMime INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/AkonadiMime>")
target_include_directories(KPim${KF_MAJOR_VERSION}AkonadiMime PUBLIC "$<BUILD_INTERFACE:${Akonadi-Mime_SOURCE_DIR}/src;${Akonadi-Mime_BINARY_DIR}/src>")

target_link_libraries(KPim${KF_MAJOR_VERSION}AkonadiMime
    PUBLIC
    KPim${KF_MAJOR_VERSION}::AkonadiCore
    PRIVATE
    KPim${KF_MAJOR_VERSION}::Mime
    KPim${KF_MAJOR_VERSION}::AkonadiWidgets
    KF${KF_MAJOR_VERSION}::KIOCore
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::XmlGui
    )

set_target_properties(KPim${KF_MAJOR_VERSION}AkonadiMime PROPERTIES
    VERSION ${AKONADIMIME_VERSION}
    SOVERSION ${AKONADIMIME_SOVERSION}
    EXPORT_NAME AkonadiMime
    )

install(TARGETS
    KPim${KF_MAJOR_VERSION}AkonadiMime
    EXPORT KPim${KF_MAJOR_VERSION}AkonadiMimeTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
    )


ecm_generate_headers(AkonadiMime_CamelCase_HEADERS
    HEADER_NAMES

    AddressAttribute
    CommandBase
    MDNStateAttribute
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
    PREFIX Akonadi
    )

install(FILES
    ${AkonadiMime_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/AkonadiMime/Akonadi
    COMPONENT Devel
    )

install(FILES
    ${AkonadiMime_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/akonadi-mime_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/AkonadiMime/akonadi
    COMPONENT Devel
    )

ecm_generate_pri_file(BASE_NAME AkonadiMime
    LIB_NAME KPim${KF_MAJOR_VERSION}AkonadiMime
    DEPS "AkonadiCore" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/AkonadiMime
    )

install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})


if (BUILD_QCH)
    ecm_add_qch(
        KPim${KF_MAJOR_VERSION}AkonadiMime_QCH
        NAME KPim${KF_MAJOR_VERSION}AkonadiMime
        BASE_NAME KPim${KF_MAJOR_VERSION}AkonadiMime
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${AkonadiMime_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt${QT_MAJOR_VERSION}Core_QCH
            Qt${QT_MAJOR_VERSION}Gui_QCH
            Qt${QT_MAJOR_VERSION}Widgets_QCH
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


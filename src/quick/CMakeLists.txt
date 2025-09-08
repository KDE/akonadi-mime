# SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
# SPDX-License-Identifier: BSD-3-Clause

ecm_add_qml_module(akonadi_mime_quick_plugin
    URI "org.kde.akonadi.mime"
    GENERATE_PLUGIN_SOURCE
)

target_sources(
    akonadi_mime_quick_plugin
    PRIVATE
        types.cpp
        types.h
)

target_link_libraries(akonadi_mime_quick_plugin PRIVATE KPim6AkonadiMime)

ecm_finalize_qml_module(akonadi_mime_quick_plugin
    DESTINATION ${KDE_INSTALL_QMLDIR}
    BUILD_SHARED_LIBS OFF
)

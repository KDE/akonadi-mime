add_definitions(-DTRANSLATION_DOMAIN=\"libakonadi-kmime5-serializer\")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(akonadi_serializer_mail MODULE)
target_sources(akonadi_serializer_mail PRIVATE
    akonadi_serializer_mail.cpp
)

ecm_qt_declare_logging_category(akonadi_serializer_mail
        HEADER akonadi_serializer_mail_debug.h
        IDENTIFIER AKONADI_SERIALIZER_MAIL_LOG
        DESCRIPTION "akonadi-mime (akonadi_serializer_mail)"
        CATEGORY_NAME org.kde.pim.akonadi_serializer_mail
        EXPORT AKONADIMIME
    )

target_link_libraries(akonadi_serializer_mail
    KF5::Mime
    KF5::AkonadiCore
    KF5::AkonadiMime
    KF5::AkonadiPrivate
)
install(TARGETS akonadi_serializer_mail
    DESTINATION ${KDE_INSTALL_PLUGINDIR}
)
install(FILES akonadi_serializer_mail.desktop
    DESTINATION ${KDE_INSTALL_DATADIR}/akonadi/plugins/serializer
)

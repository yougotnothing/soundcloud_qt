cmake_minimum_required(VERSION 3.16)

project(soundcloud_qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appsoundcloud_qt
    main.cpp
)

qt_add_qml_module(appsoundcloud_qt
    URI soundcloud_qt
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES services/authservice.h services/authservice.cpp
        SOURCES services/userservice.h services/userservice.cpp
        SOURCES structs/user.h
        SOURCES
        SOURCES data/consts.h
        SOURCES services/trackservice.h services/trackservice.cpp
        SOURCES structs/track.h
        SOURCES services/config.h
        SOURCES
        SOURCES servers/oauthserver.h servers/oauthserver.cpp
)

qt_add_resources(appsoundcloud_qt "app_resources"
    PREFIX "/"
    FILES
        config.json
)

target_link_libraries(appsoundcloud_qt
    PRIVATE
        Qt6::Quick
        Qt6::Multimedia
)

set_target_properties(appsoundcloud_qt PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS appsoundcloud_qt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

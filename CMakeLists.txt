cmake_minimum_required(VERSION 3.16)

project(soundcloud_qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appsoundcloud_qt
    main.cpp
        services/PlaylistsService.cpp
        services/PlaylistsService.h
        structs/Playlist.h
)

file(GLOB_RECURSE QML_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/components/*.qml"
)

# Convert absolute paths to relative paths for Qt resources
file(RELATIVE_PATH _qml_base "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
set(QML_FILES_RELATIVE "")
foreach(f ${QML_FILES})
    file(RELATIVE_PATH rel "${CMAKE_CURRENT_SOURCE_DIR}" "${f}")
    list(APPEND QML_FILES_RELATIVE "${rel}")
endforeach()
set(QML_FILES ${QML_FILES_RELATIVE})

qt_add_qml_module(appsoundcloud_qt
    URI soundcloud_qt
    VERSION 1.0
    QML_FILES
        Main.qml
        ${QML_FILES}
    SOURCES
        services/authservice.h services/authservice.cpp
        services/userservice.h services/userservice.cpp
        structs/user.h
        data/consts.h
        services/trackservice.h services/trackservice.cpp
        structs/track.h
        services/config.h
        servers/oauthserver.h servers/oauthserver.cpp
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

cmake_minimum_required (VERSION 3.5)

project (Minecraft-Backup LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LibArchive REQUIRED)

add_executable (Minecraft-Backup
    backup.h backup.cpp

    main.cpp
    archiveFunctions.h archiveFunctions.cpp
    commandLine.h commandLine.cpp

    utils.h
)

add_custom_target(Documentation SOURCES
    README.md
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Minecraft-Backup  PUBLIC RANDOMLY_DEBUGGING)
endif()

target_link_libraries(Minecraft-Backup  PRIVATE LibArchive::LibArchive)

install (TARGETS Minecraft-Backup
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

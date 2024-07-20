cmake_minimum_required (VERSION 3.5)

project (Minecraft-Backup LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# required for GUI/Qt
set(CMAKE_AUTOMOC ON)

find_package(LibArchive REQUIRED)

# required for GUI
find_package(Qt6 REQUIRED Quick Core)

include_directories("src")

add_library(mcBackupCore SHARED
    src/backup.h src/backup.cpp

    src/archiveFunctions.h src/archiveFunctions.cpp
    src/commandLine.h src/commandLine.cpp

    src/utils.h
)

add_executable (Minecraft-Backup
    src/main.cpp
)

add_executable (Minecraft-Backup-GUI
    src/gui/main.cpp
    src/gui/qmlguihandler.h src/gui/qmlguihandler.cpp
)

add_custom_target(Documentation SOURCES
    README.md
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Minecraft-Backup  PUBLIC RANDOMLY_DEBUGGING)
endif()

target_link_options(Minecraft-Backup-GUI PRIVATE -v)

target_link_libraries(Minecraft-Backup      PRIVATE mcBackupCore)
target_link_libraries(Minecraft-Backup-GUI  PRIVATE mcBackupCore)
target_link_libraries(Minecraft-Backup-GUI  PRIVATE Qt6::Quick Qt6::Core)

target_link_libraries(mcBackupCore  PRIVATE LibArchive::LibArchive)

install (TARGETS Minecraft-Backup mcBackupCore
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

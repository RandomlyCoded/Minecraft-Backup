cmake_minimum_required (VERSION 3.5)

project (Minecraft-Backup LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LibArchive REQUIRED)

add_library(Helper STATIC utils.h)

add_executable (Minecraft-Backup  backup.cpp)

add_executable (Minecraft-Restore restore.cpp)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Minecraft-Backup  PUBLIC RANDOMLY_DEBUGGING)
    target_compile_definitions(Minecraft-Restore PUBLIC RANDOMLY_DEBUGGING)

    target_compile_definitions(Helper           PUBLIC RANDOMLY_DEBUGGING)
endif()

target_link_libraries(Minecraft-Backup  PRIVATE Helper LibArchive::LibArchive)
target_link_libraries(Minecraft-Restore PRIVATE Helper LibArchive::LibArchive)

install (TARGETS Minecraft-Backup Minecraft-Restore
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

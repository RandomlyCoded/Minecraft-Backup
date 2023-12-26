cmake_minimum_required(VERSION 3.5)

project(Minecraft-Backup LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Minecraft-Backup backup.cpp)
add_executable(Minecraft-Restore restore.cpp)

install(TARGETS Minecraft-Backup
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

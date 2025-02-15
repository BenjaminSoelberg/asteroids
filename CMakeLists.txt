cmake_minimum_required(VERSION 3.30)
project(astroids)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)
find_package(Python3 REQUIRED)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
link_libraries(${GTK_LIBRARIES})

set(AUTO_GEN_DIR "${PROJECT_BINARY_DIR}/generated")
add_custom_command(
        OUTPUT ${AUTO_GEN_DIR}/dvg_rom.c
        COMMAND ${PYTHON3} ${CMAKE_CURRENT_SOURCE_DIR}/convert.py ${AUTO_GEN_DIR}/dvg_rom.c
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/convert.py
        COMMENT "Generate (custom command)"
)

include_directories(${AUTO_GEN_DIR})

add_executable(astroids
        asteroids.c
        asteroids.h
        dvg.c
        dvg.h
        ${AUTO_GEN_DIR}/dvg_rom.c
        dvg_rom.h
)

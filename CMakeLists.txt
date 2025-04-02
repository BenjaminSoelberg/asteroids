cmake_minimum_required(VERSION 3.30)
project(asteroids)

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

add_executable(asteroids
        asteroids.c
        asteroids.h
        dvg.c
        dvg.h
        ${AUTO_GEN_DIR}/dvg_rom.c
        dvg_rom.h
        commons.c
        commons.h
        print.c
        print.h
        game_A35131_1A.c
        game_A35131_1A.h
        msg_A35131_1B.c
        msg_A35131_1B.h
        vg_A35131_1C.c
        vg_A35131_1C.h
        boot_A35131_1E.c
        boot_A35131_1E.h
)

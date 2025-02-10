cmake_minimum_required(VERSION 3.30)
project(astroids)

set(CMAKE_CXX_STANDARD 11)

add_executable(astroids
        asteroids.c
        asteroids.h
        dvg.c
        dvg.h)

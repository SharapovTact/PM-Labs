cmake_minimum_required(VERSION 3.16)
project(PM_Labs C)

set(CMAKE_C_STANDARD 11)

include_directories(.)


add_executable(PM_Labs
        lab8/shapes.c
        lab8/shapes.h
        lab8/lt2.c
)
target_link_libraries(PM_Labs m)

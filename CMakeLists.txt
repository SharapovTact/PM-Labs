cmake_minimum_required(VERSION 4.1)
project(PM_Labs C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(lab1)
include_directories(lab2)

add_executable(PM_Labs
        lab1/konsp.c
        lab1/lw1.c
        lab1/lw2.c
        lab1/lw3.c
        lab1/lw4.c
        lab2/task.c
        lab2/lw1.c
        lab2/lw2.2.c
        lab2/lw2.3.c
        lab2/lw2.4.c
        lab2/lw2.5.c
        lab2/lw2.6.c
        lab3/lw1.c
)

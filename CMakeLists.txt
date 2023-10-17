cmake_minimum_required(VERSION 3.26)
project(Spectral)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_COMPILER "gcc")

add_executable(Spectral src/main.cpp
        src/script.h)

target_link_options(Spectral PUBLIC -Wl,-R -Wl,.)



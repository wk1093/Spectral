cmake_minimum_required(VERSION 3.26)
project(Spectral)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_COMPILER "gcc") # disable c++ stdlib
# enable -fno-rtti
add_compile_options(-fno-rtti -fno-exceptions)
add_link_options(-fno-rtti -fno-exceptions)

add_subdirectory(src)

add_executable(Spectral src/main.cpp)
target_link_options(Spectral PUBLIC -Wl,-R -Wl,.)
add_dependencies(Spectral SpectralModules)



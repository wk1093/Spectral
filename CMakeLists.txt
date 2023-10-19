cmake_minimum_required(VERSION 3.26)
project(Spectral)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-fno-exceptions)
add_link_options(-fno-exceptions)
include_directories(src/include)


add_subdirectory(src/modules)
set(CMAKE_CXX_COMPILER "/usr/bin/gcc")

add_executable(Spectral src/main.cpp)
target_link_options(Spectral PUBLIC -Wl,-R -Wl,.)
add_dependencies(Spectral SpectralModules)
set_target_properties(Spectral PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# set SCRIPT_INCLUDE_DIR to ../include define



cmake_minimum_required(VERSION 3.26)
project(Spectral)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath,lib/${SPECTRAL_FULLPLAT} -Wl,-rpath-link,lib/${SPECTRAL_FULLPLAT}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,lib/${SPECTRAL_FULLPLAT} -Wl,-rpath-link,lib/${SPECTRAL_FULLPLAT}")

include_directories(src/include)

add_subdirectory(src/modules)
add_subdirectory(src/game)

add_executable(Spectral src/main.cpp)
# add ./lib to the RPATH
add_dependencies(Spectral SpectralModules)
add_dependencies(Spectral SpectralGame)
target_link_libraries(Spectral SpectralInterface)

set_target_properties(Spectral PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(Spectral PROPERTIES SUFFIX ".${SPECTRAL_FULLPLAT}${CMAKE_EXECUTABLE_SUFFIX}")
target_compile_definitions(Spectral PUBLIC "SPECTRAL_PLATFORM=\"${SPECTRAL_FULLPLAT}\"")
# set SCRIPT_INCLUDE_DIR to ../include define


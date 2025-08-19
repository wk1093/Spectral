cmake_minimum_required(VERSION 3.26)
project(Spectral)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath,lib/${SPECTRAL_FULLPLAT} -Wl,-rpath-link,lib/${SPECTRAL_FULLPLAT}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,lib/${SPECTRAL_FULLPLAT} -Wl,-rpath-link,lib/${SPECTRAL_FULLPLAT}")
# CMP0169
cmake_policy(SET CMP0169 OLD)

include_directories(src/include)

add_subdirectory(src/modules)
add_subdirectory(src/game)

add_executable(SpectralEngine src/engine.cpp)
add_dependencies(SpectralEngine SpectralModules)
add_dependencies(SpectralEngine SpectralGame)
target_link_libraries(SpectralEngine SpectralInterface)

set_target_properties(SpectralEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(SpectralEngine PROPERTIES SUFFIX ".${SPECTRAL_FULLPLAT}${CMAKE_EXECUTABLE_SUFFIX}")
target_compile_definitions(SpectralEngine PUBLIC "SPECTRAL_PLATFORM=\"${SPECTRAL_FULLPLAT}\"")
target_include_directories(SpectralEngine PUBLIC ${CMAKE_SOURCE_DIR}/src/include ${CMAKE_SOURCE_DIR}/src/modules)

set(CMAKE_EXE_NAME "SpectralEngine.${SPECTRAL_FULLPLAT}${CMAKE_EXECUTABLE_SUFFIX}")
add_executable(Spectral src/main.cpp)
add_compile_definitions(Spectral PUBLIC "SPECTRALENGINE_EXE_NAME=\"${CMAKE_EXE_NAME}\"")
# add ./lib to the RPATH
add_dependencies(Spectral SpectralEngine)
target_link_libraries(Spectral SpectralInterface)

set_target_properties(Spectral PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(Spectral PROPERTIES SUFFIX ".${SPECTRAL_FULLPLAT}${CMAKE_EXECUTABLE_SUFFIX}")
target_compile_definitions(Spectral PUBLIC "SPECTRAL_PLATFORM=\"${SPECTRAL_FULLPLAT}\"")
target_include_directories(Spectral PUBLIC ${CMAKE_SOURCE_DIR}/src/include ${CMAKE_SOURCE_DIR}/src/modules)


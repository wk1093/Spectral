
include(FetchContent)

# make sure glfw is linked dynamically (to prevent context errors)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(CMAKE_DEBUG_POSTFIX "")
set(EOGLL_DYNAMIC ON CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 17)

# make it so all included libs will output their dynamic libs to the bin/lib folder
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.0
)
FetchContent_MakeAvailable(sfml)
set_target_properties(sfml-graphics PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(sfml-window PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(sfml-system PROPERTIES POSITION_INDEPENDENT_CODE ON)

FetchContent_Declare(
        eogll
        GIT_REPOSITORY https://github.com/wk1093/EOGLL.git
)
FetchContent_MakeAvailable(eogll)

Find_Package(OpenGL REQUIRED)

macro(add_module mod impl source)
    add_library(${mod}_${impl} SHARED ${source})
    set_target_properties(${mod}_${impl} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules/${mod}")
    set_target_properties(${mod}_${impl} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules/${mod}")
    set_target_properties(${mod}_${impl} PROPERTIES PREFIX "")
    set_target_properties(${mod}_${impl} PROPERTIES SUFFIX ".splmod")
    # debug
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(${mod}_${impl} PUBLIC "DEBUG_BUILD")
    endif()
endmacro()


add_module(scrld cpp scrld/cppscript.cpp)

add_module(win glfw_gl win/glfwwin.cpp)
target_link_libraries(win_glfw_gl glfw)
target_compile_definitions(win_glfw_gl PUBLIC "SPECTRAL_GLFW_OPENGL")
target_include_directories(win_glfw_gl PUBLIC ${glfw_BINARY_DIR}/src)

add_module(win glfw_noapi win/glfwwin.cpp)
target_link_libraries(win_glfw_noapi glfw)
target_compile_definitions(win_glfw_noapi PUBLIC "SPECTRAL_GLFW_NOAPI")
target_include_directories(win_glfw_noapi PUBLIC ${glfw_BINARY_DIR}/src)

add_module(win sf win/sfwin.cpp)
target_link_libraries(win_sf sfml-graphics sfml-window sfml-system)

add_module(win eogll win/eogllwin.cpp)
target_link_libraries(win_eogll eogll)
target_include_directories(win_eogll PUBLIC ${glfw_BINARY_DIR}/src)

add_module(gfx eogll gfx/eogll.cpp)
target_link_libraries(gfx_eogll eogll opengl32)

add_module(gfx glad gfx/glad.cpp)
target_link_libraries(gfx_glad glad opengl32)

add_module(gfx d3d11_1 gfx/d3d11_1.cpp)
target_link_libraries(gfx_d3d11_1 d3d11 d3dcompiler dxgi)

add_module(shdr spsl_glsl shdr/spsl.cpp)
target_compile_definitions(shdr_spsl_glsl PUBLIC "SPECTRAL_OUTPUT_GLSL")

add_module(shdr spsl_hlsl shdr/spsl.cpp)
target_compile_definitions(shdr_spsl_hlsl PUBLIC "SPECTRAL_OUTPUT_HLSL")


add_custom_target(
        SpectralModules ALL
        DEPENDS scrld_cpp
        DEPENDS win_glfw_gl
        DEPENDS win_glfw_noapi
        DEPENDS win_sf
        DEPENDS win_eogll
        DEPENDS gfx_eogll
        DEPENDS gfx_glad
        DEPENDS gfx_d3d11_1
        DEPENDS shdr_spsl_glsl
        DEPENDS shdr_spsl_hlsl
)

file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")

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

macro(add_module name source)
    add_library(${name} SHARED ${source})
    set_target_properties(${name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
    set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
    set_target_properties(${name} PROPERTIES PREFIX "")
    set_target_properties(${name} PROPERTIES SUFFIX ".splmod")
endmacro()


add_module(scrld_cpp scrld/cppscript.cpp)

add_module(win_glfw win/glfwwin.cpp)
target_link_libraries(win_glfw glfw)
target_include_directories(win_glfw PUBLIC ${glfw_BINARY_DIR}/src)

add_module(win_sf win/sfwin.cpp)
target_link_libraries(win_sf sfml-graphics sfml-window sfml-system)

add_module(win_eogll win/eogllwin.cpp)
target_link_libraries(win_eogll eogll)
target_include_directories(win_eogll PUBLIC ${glfw_BINARY_DIR}/src)

add_module(gfx_eogll gfx/eogll.cpp)
target_link_libraries(gfx_eogll eogll)

add_module(gfx_sf gfx/sf.cpp)
target_link_libraries(gfx_sf sfml-graphics sfml-window sfml-system opengl32)

add_module(gfx_glad gfx/glad.cpp)
target_link_libraries(gfx_glad glad opengl32)

add_custom_target(
        SpectralModules ALL
        DEPENDS scrld_cpp
        DEPENDS win_glfw
        DEPENDS win_sf
        DEPENDS win_eogll
        DEPENDS gfx_eogll
        DEPENDS gfx_sf
        DEPENDS gfx_glad
)

file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")

include(FetchContent)

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

set(CMAKE_DEBUG_POSTFIX "")

add_library(scrld_cpp SHARED scrld/cppscript.cpp)
set_target_properties(scrld_cpp PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
set_target_properties(scrld_cpp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
set_target_properties(scrld_cpp PROPERTIES PREFIX "")
set_target_properties(scrld_cpp PROPERTIES SUFFIX ".splmod")

add_library(win_glfw SHARED win/glfwwin.cpp)
set_target_properties(win_glfw PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
set_target_properties(win_glfw PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
set_target_properties(win_glfw PROPERTIES PREFIX "")
set_target_properties(win_glfw PROPERTIES SUFFIX ".splmod")
target_link_libraries(win_glfw glfw)
target_include_directories(win_glfw PUBLIC ${glfw_BINARY_DIR}/src)

add_library(win_sf SHARED win/sfwin.cpp)
set_target_properties(win_sf PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
set_target_properties(win_sf PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
set_target_properties(win_sf PROPERTIES PREFIX "")
set_target_properties(win_sf PROPERTIES SUFFIX ".splmod")
target_link_libraries(win_sf sfml-graphics sfml-window sfml-system)

add_library(win_eogll SHARED win/eogllwin.cpp)
set_target_properties(win_eogll PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
set_target_properties(win_eogll PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
set_target_properties(win_eogll PROPERTIES PREFIX "")
set_target_properties(win_eogll PROPERTIES SUFFIX ".splmod")
target_link_libraries(win_eogll eogll)
target_include_directories(win_eogll PUBLIC ${glfw_BINARY_DIR}/src)

add_custom_target(
        SpectralModules ALL
        DEPENDS scrld_cpp
        DEPENDS win_glfw
        DEPENDS win_sf
        DEPENDS win_eogll
)
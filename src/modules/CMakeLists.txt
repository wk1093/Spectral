
add_library(cppscript SHARED scriptloading/cppscript.cpp)
set_target_properties(cppscript PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/modules)
set_target_properties(cppscript PROPERTIES PREFIX "")

add_library(glfwwin SHARED window/glfwwin.cpp)
set_target_properties(glfwwin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/modules)
set_target_properties(glfwwin PROPERTIES PREFIX "")
# add glfw using fetchcontent

include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

target_link_libraries(glfwwin glfw)

add_library(sftgui SHARED engineui/sftgui.cpp)
set_target_properties(sftgui PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/modules)
set_target_properties(sftgui PROPERTIES PREFIX "")

add_custom_target(
        SpectralModules ALL
        DEPENDS ${CMAKE_SOURCE_DIR}/bin/modules/cppscript.so
        DEPENDS ${CMAKE_SOURCE_DIR}/bin/modules/glfwwin.so
        DEPENDS ${CMAKE_SOURCE_DIR}/bin/modules/sftgui.so
)
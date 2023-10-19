
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


add_library(cppscript SHARED scriptloading/cppscript.cpp)
set_target_properties(cppscript PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/modules)
set_target_properties(cppscript PROPERTIES PREFIX "")

add_library(glfwwin SHARED window/glfwwin.cpp)
set_target_properties(glfwwin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/modules)
set_target_properties(glfwwin PROPERTIES PREFIX "")
target_link_libraries(glfwwin glfw)
target_include_directories(glfwwin PUBLIC ${glfw_BINARY_DIR}/src)

add_library(sfwin SHARED window/sfwin.cpp)
set_target_properties(sfwin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/modules)
set_target_properties(sfwin PROPERTIES PREFIX "")
target_link_libraries(sfwin sfml-graphics sfml-window sfml-system)

if (WIN32)
    add_custom_command(
            TARGET sfml-graphics
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:sftgui>
            VERBATIM)
endif ()

add_custom_target(
        SpectralModules ALL
        DEPENDS ${CMAKE_SOURCE_DIR}/bin/modules/cppscript.so
        DEPENDS ${CMAKE_SOURCE_DIR}/bin/modules/sfwin.so
        DEPENDS ${CMAKE_SOURCE_DIR}/bin/modules/glfwwin.so
)
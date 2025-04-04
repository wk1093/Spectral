
include(FetchContent)

set(SPEC_GLFW ON CACHE BOOL "" FORCE)
set(SPEC_EOGLL OFF CACHE BOOL "" FORCE)
set(SPEC_SFML OFF CACHE BOOL "" FORCE)
set(SPEC_GLAD ON CACHE BOOL "" FORCE)
set(SPEC_DX11 ON CACHE BOOL "" FORCE)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(CMAKE_DEBUG_POSTFIX "")
set(EOGLL_DYNAMIC ON CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_HOGLL OFF CACHE BOOL "" FORCE)
set(BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(LIBZIP_DO_INSTALL OFF CACHE BOOL "" FORCE)
set(ZLIB_VERBOSE OFF CACHE BOOL "" FORCE)
set(BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(BUILD_REGRESS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_DOC OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(BUILD_OSSFUZZ OFF CACHE BOOL "" FORCE)
set(GLAD_DYNAMIC ON CACHE BOOL "" FORCE)
set(STB_DYNAMIC ON CACHE BOOL "" FORCE)

if (NOT WIN32)
    set(SPEC_DX11 OFF CACHE BOOL "" FORCE)
endif()


set(CMAKE_CXX_STANDARD 17)

# the platform is determined by the CMAKE_SYSTEM_NAME variable
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(SPECTRAL_PLATFORM "win")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(SPECTRAL_PLATFORM "lin")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(SPECTRAL_PLATFORM "mac")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(SPECTRAL_PLATFORM "and")
    message(WARNING "Android is not tested and may not work")
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(SPECTRAL_PLATFORM "ios")
    message(WARNING "iOS is not tested and may not work")
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    set(SPECTRAL_PLATFORM "bsd")
    message(WARNING "FreeBSD is not tested and may not work")
elseif(CMAKE_SYSTEM_NAME STREQUAL "CYGWIN")
    set(SPECTRAL_PLATFORM "cyg")
    message(WARNING "Cygwin is not tested and may not work")
else()
    set(SPECTRAL_PLATFORM "unk")
    message(WARNING "Unknown platform, this may not work")
endif()

# add the platform onto it
set(SPECTRAL_FULLPLAT "${SPECTRAL_PLATFORM}_${CMAKE_SYSTEM_PROCESSOR}" CACHE STRING "The full platform name")

# make it so all included libs will output their dynamic libs to the bin/lib folder
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib/${SPECTRAL_FULLPLAT}/" CACHE PATH "" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib/${SPECTRAL_FULLPLAT}/" CACHE PATH "" FORCE)
set(zlib_BINARY_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} CACHE PATH "" FORCE)

find_package(ZLIB REQUIRED)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
)
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.0
)
FetchContent_Declare(
    eogll
    GIT_REPOSITORY https://github.com/wk1093/EOGLL.git
)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)


FetchContent_Declare(
    zip
    GIT_REPOSITORY https://github.com/nih-at/libzip.git
    GIT_TAG v1.11.3
)
FetchContent_MakeAvailable(zip)
FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://github.com/freetype/freetype.git
    GIT_TAG VER-2-13-3
)
FetchContent_MakeAvailable(freetype)
# we need a command to run after zlib is built (only on windows) to copy the dlls to the bin/lib folder
if(SPEC_GLFW)
    FetchContent_MakeAvailable(glfw)
endif()
if(SPEC_SFML)
    FetchContent_MakeAvailable(sfml)
    set_target_properties(sfml-graphics PROPERTIES POSITION_INDEPENDENT_CODE ON)
    set_target_properties(sfml-window PROPERTIES POSITION_INDEPENDENT_CODE ON)
    set_target_properties(sfml-system PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
if(SPEC_EOGLL)
    FetchContent_MakeAvailable(eogll)
elseif(SPEC_GLAD)
    # make sure eogll is cloned, but can't use makeavailable because it will build it
    if (NOT EXISTS ${eogll_SOURCE_DIR}/CMakeLists.txt)
        FetchContent_Populate(eogll)
    endif()
    # Eogll normally provides GLAD, but if it is not being used, we need to provide it ourselves
    add_subdirectory(${eogll_SOURCE_DIR}/thirdparty/glad ${CMAKE_BINARY_DIR}/glad)
endif()

Find_Package(OpenGL REQUIRED)

if(WIN32)
    set(OPENGL_LIB opengl32)
else()
    set(OPENGL_LIB GL)
endif()

if (SPEC_DX11)
    # This will just see if we can find the dx11 dlls and copy them to the bin/lib folder
    # This requires that the developer has the Windows SDK installed, but should make it so the user doesn't need to

    # assuming the dev has them in sys32
    file(GLOB dx11_dlls "C:/Windows/System32/d3d11.dll" "C:/Windows/System32/d3dcompiler_47.dll" "C:/Windows/System32/dxgi.dll")
    foreach(dll ${dx11_dlls})
        file(COPY ${dll} DESTINATION "${CMAKE_SOURCE_DIR}/bin/lib/${SPECTRAL_FULLPLAT}/")
    endforeach()
endif()

# OpenAL
FetchContent_Declare(
    openal
    GIT_REPOSITORY https://github.com/kcat/openal-soft.git
    GIT_TAG 1.24.2
)
FetchContent_MakeAvailable(openal)


macro(add_module mod impl source)
    add_library(${mod}_${impl} SHARED ${source})
    set_target_properties(${mod}_${impl} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules/${SPECTRAL_FULLPLAT}/${mod}")
    set_target_properties(${mod}_${impl} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules/${SPECTRAL_FULLPLAT}/${mod}")
    set_target_properties(${mod}_${impl} PROPERTIES PREFIX "")
    set_target_properties(${mod}_${impl} PROPERTIES SUFFIX ".splmod")
    target_compile_definitions(${mod}_${impl} PUBLIC "SPECTRAL_PLATFORM=\"${SPECTRAL_FULLPLAT}\"")
    # debug
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(${mod}_${impl} PUBLIC "DEBUG_BUILD")
    endif()
endmacro()


add_module(scrld cpp scrld/cppscript.cpp)

if(SPEC_GLFW)
    add_module(win glfw_gl win/glfwwin.cpp)
    target_link_libraries(win_glfw_gl glfw)
    target_compile_definitions(win_glfw_gl PUBLIC "SPECTRAL_GLFW_OPENGL")
    target_include_directories(win_glfw_gl PUBLIC ${glfw_BINARY_DIR}/src)

    add_module(win glfw_noapi win/glfwwin.cpp)
    target_link_libraries(win_glfw_noapi glfw)
    target_compile_definitions(win_glfw_noapi PUBLIC "SPECTRAL_GLFW_NOAPI")
    target_include_directories(win_glfw_noapi PUBLIC ${glfw_BINARY_DIR}/src)
endif()

if(SPEC_SFML)
    add_module(win sf win/sfwin.cpp)
    target_link_libraries(win_sf sfml-graphics sfml-window sfml-system)
endif()
if(SPEC_EOGLL)
    add_module(win eogll win/eogllwin.cpp)
    target_link_libraries(win_eogll eogll)
    target_include_directories(win_eogll PUBLIC ${glfw_BINARY_DIR}/src)

    add_module(gfx eogll gfx/eogll.cpp)
    target_link_libraries(gfx_eogll eogll ${OPENGL_LIB})
else()
    add_subdirectory(${eogll_SOURCE_DIR}/thirdparty/stb ${CMAKE_BINARY_DIR}/stb)
endif()

if(SPEC_GLAD)
    add_module(gfx glad gfx/glad.cpp)
    target_link_libraries(gfx_glad glad ${OPENGL_LIB})
endif()
if(SPEC_DX11)
    add_module(gfx d3d11_1 gfx/d3d11_1.cpp)
    target_link_libraries(gfx_d3d11_1 d3d11 d3dcompiler dxgi)
endif()

add_module(shdr spsl_glsl shdr/spsl.cpp)
target_compile_definitions(shdr_spsl_glsl PUBLIC "SPECTRAL_OUTPUT_GLSL")

add_module(shdr spsl_hlsl shdr/spsl.cpp)
target_compile_definitions(shdr_spsl_hlsl PUBLIC "SPECTRAL_OUTPUT_HLSL")

add_module(tex stb tex/stb.cpp)
target_link_libraries(tex_stb stb)

add_module(wrld main wrld/wrldmain.cpp)
target_link_libraries(wrld_main nlohmann_json zip)

add_module(text ft text/freetype.cpp)
target_link_libraries(text_ft freetype)

add_module(aud openal aud/openal.cpp)
target_link_libraries(aud_openal OpenAL)

add_custom_target(
        SpectralModules ALL
        DEPENDS scrld_cpp
        DEPENDS shdr_spsl_glsl
        DEPENDS shdr_spsl_hlsl
)
if(SPEC_GLFW)
    add_dependencies(SpectralModules win_glfw_gl win_glfw_noapi)
endif()
if(SPEC_SFML)
    add_dependencies(SpectralModules win_sf)
endif()
if(SPEC_EOGLL)
    add_dependencies(SpectralModules win_eogll gfx_eogll)
endif()
if(SPEC_GLAD)
    add_dependencies(SpectralModules gfx_glad)
endif()
if(SPEC_DX11)
    add_dependencies(SpectralModules gfx_d3d11_1)
endif()

add_library(SpectralInterface INTERFACE)

file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")
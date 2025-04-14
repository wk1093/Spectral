
add_library(game_game SHARED src/game.cpp)
set_target_properties(game_game PROPERTIES PREFIX "")
set_target_properties(game_game PROPERTIES SUFFIX ".splmod")
set_target_properties(game_game PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules/${SPECTRAL_FULLPLAT}/game")
set_target_properties(game_game PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules/${SPECTRAL_FULLPLAT}/game")
target_compile_definitions(game_game PUBLIC "SPECTRAL_PLATFORM=\"${SPECTRAL_FULLPLAT}\"")
target_include_directories(game_game PUBLIC "../modules")


include(CMakeParseArguments)

# Function to wrap a given string into multiple lines at the given column position.
# Parameters:
#   VARIABLE    - The name of the CMake variable holding the string.
#   AT_COLUMN   - The column position at which string will be wrapped.
function(WRAP_STRING)
    set(oneValueArgs VARIABLE AT_COLUMN)
    cmake_parse_arguments(WRAP_STRING "${options}" "${oneValueArgs}" "" ${ARGN})

    string(LENGTH ${${WRAP_STRING_VARIABLE}} stringLength)
    math(EXPR offset "0")

    while(stringLength GREATER 0)

        if(stringLength GREATER ${WRAP_STRING_AT_COLUMN})
            math(EXPR length "${WRAP_STRING_AT_COLUMN}")
        else()
            math(EXPR length "${stringLength}")
        endif()

        string(SUBSTRING ${${WRAP_STRING_VARIABLE}} ${offset} ${length} line)
        set(lines "${lines}\n${line}")

        math(EXPR stringLength "${stringLength} - ${length}")
        math(EXPR offset "${offset} + ${length}")
    endwhile()

    set(${WRAP_STRING_VARIABLE} "${lines}" PARENT_SCOPE)
endfunction()

# Function to embed contents of a file as byte array in C/C++ header file(.h). The header file
# will contain a byte array and integer variable holding the size of the array.
# Parameters
#   SOURCE_FILE     - The path of source file whose contents will be embedded in the header file.
#   VARIABLE_NAME   - The name of the variable for the byte array. The string "_SIZE" will be append
#                     to this name and will be used a variable name for size variable.
#   HEADER_FILE     - The path of header file.
#   APPEND          - If specified appends to the header file instead of overwriting it
#   NULL_TERMINATE  - If specified a null byte(zero) will be append to the byte array. This will be
#                     useful if the source file is a text file and we want to use the file contents
#                     as string. But the size variable holds size of the byte array without this
#                     null byte.
# Usage:
#   bin2h(SOURCE_FILE "Logo.png" HEADER_FILE "Logo.h" VARIABLE_NAME "LOGO_PNG")
function(BIN2H)
    set(options APPEND NULL_TERMINATE)
    set(oneValueArgs SOURCE_FILE VARIABLE_NAME HEADER_FILE)
    cmake_parse_arguments(BIN2H "${options}" "${oneValueArgs}" "" ${ARGN})

    # reads source file contents as hex string
    file(READ ${BIN2H_SOURCE_FILE} hexString HEX)
    string(LENGTH ${hexString} hexStringLength)

    # appends null byte if asked
    if(BIN2H_NULL_TERMINATE)
        set(hexString "${hexString}00")
    endif()

    # wraps the hex string into multiple lines at column 32(i.e. 16 bytes per line)
    # wrap_string(VARIABLE hexString AT_COLUMN 32)
    math(EXPR arraySize "${hexStringLength} / 2")

    # adds '0x' prefix and comma suffix before and after every byte respectively
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " arrayValues ${hexString})
    # removes trailing comma
    string(REGEX REPLACE ", $" "" arrayValues ${arrayValues})

    # converts the variable name into proper C identifier
    string(MAKE_C_IDENTIFIER "${BIN2H_VARIABLE_NAME}" BIN2H_VARIABLE_NAME)
    # declares byte array and the length variables
    set(arrayDefinition "extern \"C\" const uint8_t ${BIN2H_VARIABLE_NAME}[] = { ${arrayValues} };")
    set(arraySizeDefinition "extern \"C\" const size_t ${BIN2H_VARIABLE_NAME}_size = ${arraySize};")

    set(declarations "${arrayDefinition}\n\n${arraySizeDefinition}\n\n")

    if(BIN2H_APPEND)
        file(APPEND ${BIN2H_HEADER_FILE} "${declarations}")
    else()
        file(WRITE ${BIN2H_HEADER_FILE} "${declarations}")
    endif()
endfunction()



# convert every file inside the "assets" directory to a C++ variable definition
# these should all be compiled into a single dll file that is the asset_asset module
file(GLOB_RECURSE ASSETS_FILES "assets/*")
# the names of the variables should be relative to the assets folder. for example "assets/textures/test.png" -> "asset_textures_test_png" and "asset_textures_test_png_size"

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/assets")
# make an empty cpp file called assets.cpp in the assets folder
# not empty, we need to include stdint
file(WRITE "${CMAKE_BINARY_DIR}/assets/assets.cpp" "#include <stdint.h>\n")
message("Generating assets.cpp file")
set(SPIRV_COMPILER "glslc")
set(SPIRV_COMPILER_FLAGS --target-env=opengl -fentry-point=main -O)

add_custom_target(assets_target DEPENDS "${ASSETS_FILES}")

foreach(ASSET_FILE ${ASSETS_FILES})
    file(RELATIVE_PATH ASSET_NAME "${CMAKE_CURRENT_SOURCE_DIR}/assets" ${ASSET_FILE})
    # if the file is under the shader/ directory, create a spirv file with the exact same name, but with the .spv extension
    if(ASSET_NAME MATCHES "shader/.*\.vert" OR ASSET_NAME MATCHES "shader/.*\.frag")
        set(OLD_ASSET_NAME ${ASSET_NAME})
        set(OLD_ASSET_FILE ${ASSET_FILE})
        set(ASSET_NAME ${ASSET_NAME}.spv)
        set(ASSET_FILE "${CMAKE_BINARY_DIR}/assets/${ASSET_NAME}")

        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/assets/shader")
        # add_custom_command(
        #     COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/shader"
        #     COMMAND ${SPIRV_COMPILER} -o "${ASSET_FILE}" "${OLD_ASSET_FILE}"
        #     DEPENDS "${OLD_ASSET_FILE}"
        #     COMMENT "Compiling ${OLD_ASSET_NAME} to SPIR-V"
        #     VERBATIM
        # )
        execute_process(
            COMMAND ${SPIRV_COMPILER} -o "${ASSET_FILE}" "${OLD_ASSET_FILE}" ${SPIRV_COMPILER_FLAGS}
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/assets/shader"
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output
            ERROR_VARIABLE error
        )
        if(result EQUAL 0)
            message("Compiled ${OLD_ASSET_NAME} to SPIR-V")
        else()
            message(FATAL_ERROR "Failed to compile ${OLD_ASSET_NAME} to SPIR-V: ${error}")
        endif()
    endif()
    message("  ${ASSET_NAME}")
    BIN2H(SOURCE_FILE ${ASSET_FILE} VARIABLE_NAME "asset_${ASSET_NAME}" HEADER_FILE "${CMAKE_BINARY_DIR}/assets/assets.cpp" APPEND)
endforeach()

add_library(game_assets SHARED "${CMAKE_BINARY_DIR}/assets/assets.cpp")
add_dependencies(game_assets assets_target)

set_target_properties(game_assets PROPERTIES PREFIX "")
set_target_properties(game_assets PROPERTIES SUFFIX ".splmod")
set_target_properties(game_assets PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules/${SPECTRAL_FULLPLAT}/game")
set_target_properties(game_assets PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules/${SPECTRAL_FULLPLAT}/game")
target_compile_definitions(game_assets PUBLIC "SPECTRAL_PLATFORM=\"${SPECTRAL_FULLPLAT}\"")
target_include_directories(game_assets PUBLIC "../modules")

# SpectralGame target that links the game module and the assets module
add_custom_target(SpectralGame DEPENDS game_game game_assets)

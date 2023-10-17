
add_library(cppscript SHARED scriptloading/cppscript.cpp)
set_target_properties(cppscript PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/modules)
set_target_properties(cppscript PROPERTIES PREFIX "")



add_custom_target(
        SpectralModules ALL
        DEPENDS ${CMAKE_SOURCE_DIR}/modules/cppscript.so
)
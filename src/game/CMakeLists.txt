
add_library(game_game SHARED game.cpp)
set_target_properties(game_game PROPERTIES PREFIX "")
set_target_properties(game_game PROPERTIES SUFFIX ".splmod")
set_target_properties(game_game PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules/${SPECTRAL_FULLPLAT}/game")
set_target_properties(game_game PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/modules/${SPECTRAL_FULLPLAT}/game")
target_compile_definitions(game_game PUBLIC "SPECTRAL_PLATFORM=\"${SPECTRAL_FULLPLAT}\"")
target_include_directories(game_game PUBLIC "../modules")
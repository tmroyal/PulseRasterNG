cmake_minimum_required(VERSION 3.16)
project(deckproto C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib 4.5 REQUIRED) # from brew
find_package(PkgConfig REQUIRED)

add_subdirectory(sol2)        # provides target: sol2::sol2
add_subdirectory(libpd)
add_subdirectory(readerwriterqueue)
add_subdirectory(libremidi)

add_executable(
    deckproto
    main.cpp
    audio_engine/pdEngine.cpp
    audio_engine/pdApi.cpp
    audio_engine/pdMidi.cpp
    visual_engine/VisualEngine.cpp
    visual_engine/GraphicsApi.cpp
    script_runner/ScriptRunner.cpp
    timing_api/Scheduler.cpp
    controller_engine/MidiManager.cpp
)
# Lua from system
find_package(Lua 5.4 REQUIRED)

target_include_directories(deckproto PRIVATE audio_engine readerwriterqueue)
target_link_libraries(deckproto PRIVATE sol2::sol2 ${LUA_LIBRARIES})
target_include_directories(deckproto PRIVATE ${LUA_INCLUDE_DIR})

target_include_directories(deckproto PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/libpd_wrapper
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src
)
target_link_libraries(deckproto PRIVATE raylib libpd libremidi)

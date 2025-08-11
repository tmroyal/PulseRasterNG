cmake_minimum_required(VERSION 3.16)
project(deckproto C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib 4.5 REQUIRED) # from brew
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBLO REQUIRED liblo)

add_executable(
    deckproto
    main.cpp
    audio_engine/SC_Server.cpp
)
target_include_directories(deckproto PRIVATE audio_engine)

if (TARGET PkgConfig::LIBLO)
  target_link_libraries(deckproto PRIVATE raylib PkgConfig::LIBLO)
else()
  target_include_directories(deckproto PRIVATE ${LIBLO_INCLUDE_DIRS})
  target_link_directories(deckproto PRIVATE ${LIBLO_LIBRARY_DIRS})
  target_link_libraries(deckproto PRIVATE raylib ${LIBLO_LIBRARIES} ${LIBLO_LDFLAGS_OTHER})
endif()
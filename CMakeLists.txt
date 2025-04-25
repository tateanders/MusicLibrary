cmake_minimum_required(VERSION 3.15)
project(my_clay_app C)

set(CMAKE_C_STANDARD 99)

# Find Raylib using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED raylib)

# Include directories for Raylib and Clay
include_directories(
    ${RAYLIB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/clay
    ${CMAKE_CURRENT_SOURCE_DIR}/renderers/raylib
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
)

# Link Raylib and tell it where to find it (Homebrew default location)
link_directories(/opt/homebrew/lib)

# Define sources
set(CLAY_SOURCES
    src/main.c
    clay/clay.h
    renderers/raylib/clay_renderer_raylib.h
    resources/SpecialGothicExpandedOne-Regular.ttf
)

# Create the executable
add_executable(my_clay_app ${CLAY_SOURCES})

# Link Raylib and macOS frameworks
target_link_libraries(my_clay_app
    raylib
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreAudio"
    "-framework CoreVideo"
)

add_custom_command(
    TARGET my_clay_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
)
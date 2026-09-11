cmake_minimum_required(VERSION 3.16)
project(chip8 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    main.cpp
    loadRom.cpp
)

# Create executable
add_executable(chip8 ${SOURCES})

# Find SFML package
# This will look for SFML in standard locations
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Link SFML libraries
target_link_libraries(chip8 PRIVATE sfml-graphics sfml-window sfml-system)

# For Windows with static linking, uncomment below:
# target_compile_definitions(chip8 PRIVATE SFML_STATIC)

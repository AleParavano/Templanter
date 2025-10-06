cmake_minimum_required(VERSION 3.10)
project(TEMPLANTER)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML (Linux system install)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Collect all source files EXCEPT main files
file(GLOB_RECURSE SOURCES 
    "src/Core/*.cpp"
    "src/Patterns/*/*.cpp"
    "src/Greenhouse/*.cpp"
    "src/Staff/*.cpp"
    "src/Customer/*.cpp"
    "src/Shop/*.cpp"
    "src/Graphics/*.cpp"
    "src/Graphics/UI/*.cpp"
)

# ============================================
# CHOOSE WHICH MAIN TO USE
# ============================================
# Uncomment ONE of these lines:

# list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")              # Test suite
list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/src/game_main.cpp")          # Game

# ============================================

# Create executable
add_executable(TEMPLANTER ${SOURCES})

# Include directories
target_include_directories(TEMPLANTER PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Core
    ${CMAKE_SOURCE_DIR}/src/Patterns
    ${CMAKE_SOURCE_DIR}/src/Patterns/Memento
    ${CMAKE_SOURCE_DIR}/src/Patterns/Observer
    ${CMAKE_SOURCE_DIR}/src/Patterns/Mediator
    ${CMAKE_SOURCE_DIR}/src/Patterns/Factory
    ${CMAKE_SOURCE_DIR}/src/Patterns/Template
    ${CMAKE_SOURCE_DIR}/src/Patterns/Decorator
    ${CMAKE_SOURCE_DIR}/src/Patterns/State
    ${CMAKE_SOURCE_DIR}/src/Patterns/Command
    ${CMAKE_SOURCE_DIR}/src/Patterns/Visitor
    ${CMAKE_SOURCE_DIR}/src/Patterns/Composite
    ${CMAKE_SOURCE_DIR}/src/Patterns/Iterator
    ${CMAKE_SOURCE_DIR}/src/Greenhouse
    ${CMAKE_SOURCE_DIR}/src/Staff
    ${CMAKE_SOURCE_DIR}/src/Customer
    ${CMAKE_SOURCE_DIR}/src/Shop
    ${CMAKE_SOURCE_DIR}/src/Graphics
    ${CMAKE_SOURCE_DIR}/src/Graphics/UI
)

# Link SFML
target_link_libraries(TEMPLANTER 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Link filesystem
target_link_libraries(TEMPLANTER stdc++fs)

# Copy assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Copy saves directory structure
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/saves)

# Set output directory
set_target_properties(TEMPLANTER PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Compiler warnings (helpful for debugging)
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(TEMPLANTER PRIVATE -Wall -Wextra)
endif()
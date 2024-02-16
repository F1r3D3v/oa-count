cmake_minimum_required(VERSION 3.15)

# Set the project name
project(oa-count)

# Set C Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set the source and include path
set(SOURCE_PATH "src")
set(INCLUDE_PATH "include")

# Collect up the source files
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS "${SOURCE_PATH}/*.c")

# Set compiler flags
if (CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
    # Lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
    
    if (CMAKE_BUILD_TYPE MATCHES "Release")
        # Add compiler optimizations
        add_compile_options(-O2)
    endif()
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    # Warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
    
    # Disable secure warnings
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    if (CMAKE_BUILD_TYPE MATCHES "Release")
        # Add compiler optimizations
        add_compile_options(/O2)
    endif()
else()
  message(FATAL_ERROR "Unsupported compiler!")
endif()

# Add an executable
add_executable("${CMAKE_PROJECT_NAME}" "${SOURCE_FILES}")

# Specify include directories
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${INCLUDE_PATH}")
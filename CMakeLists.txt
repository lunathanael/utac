cmake_minimum_required(VERSION 3.10)
project(utac CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(UTAC_SOURCES 
    "src/main.cpp"
    "src/engines.cpp"
    "src/eval.cpp"
    "src/moves.cpp"
    "src/perft.cpp"
    "src/utils.cpp"
)

# Header files
set(UTAC_HEADERS
    "src/engines.hpp"
    "src/eval.hpp"
    "src/moves.hpp"
    "src/perft.hpp"
    "src/types.hpp"
    "src/utils.hpp"
)

# Compiler flags
if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
endif()

# Add library
add_library(utac STATIC ${UTAC_SOURCES})

# Include directories
target_include_directories(utac PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set public headers
set_target_properties(utac PROPERTIES PUBLIC_HEADER "${UTAC_HEADERS}")

# Link libraries
target_link_libraries(utac)

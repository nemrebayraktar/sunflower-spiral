cmake_minimum_required(VERSION 3.10)
project(sunflower_spiral)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gorgon is the course library (https://github.com/darkgazeorg/Gorgon).
# If it is installed, find_package will pick it up. You can also point
# CMake at a local build with: -DGORGON_ROOT=/path/to/Gorgon
if(DEFINED ENV{GORGON_ROOT} AND NOT GORGON_ROOT)
    set(GORGON_ROOT $ENV{GORGON_ROOT})
endif()
if(GORGON_ROOT)
    list(APPEND CMAKE_PREFIX_PATH "${GORGON_ROOT}")
endif()

find_package(Gorgon_gl QUIET)
if(Gorgon_gl_FOUND)
    set(GORGON_TARGET Gorgon::Gorgon_gl)
else()
    find_package(Gorgon REQUIRED)
    set(GORGON_TARGET Gorgon::Gorgon)
endif()

add_executable(sunflower_spiral sunflower_spiral.cpp)
target_link_libraries(sunflower_spiral PRIVATE ${GORGON_TARGET})

cmake_minimum_required(VERSION 3.16)
project(MAEDM-OCM)

# Default build
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, defaulting to Release")

    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build"
        FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------
# Configuracion de C++
# --------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags de compilacion
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -w")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

set(SOURCES

        # source code files
        src/crossoverCX.cpp
        src/evaluate.cpp
        src/getDistance.cpp
        src/Individual.h src/Individual.cpp
        src/initializeHeuristic.cpp
        src/Instance.h src/Instance.cpp
        src/intensify.cpp
        src/intensify_edges.cpp
        src/MA.h src/MA.cpp
        src/Problem.h src/ProblemLL.h
        src/solver.cpp
        src/utils/utils.h src/utils/utils.cpp
)

add_executable(solver
       ${SOURCES}
)

# Include directories
target_include_directories(solver PRIVATE
    src
    src/nlohmann
)
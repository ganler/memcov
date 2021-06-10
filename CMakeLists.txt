CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(memcov)

OPTION(MEMCOV_WITH_EXAMPLE OFF)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

ADD_LIBRARY(memcov-container STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/memcov.cpp)

# Define the coverage behavior.
ADD_LIBRARY(memcov SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/trace.cpp)
TARGET_LINK_LIBRARIES(memcov memcov-container)
TARGET_COMPILE_OPTIONS(memcov PRIVATE -fsanitize-coverage=trace-pc-guard)

# Example code.
IF(MEMCOV_WITH_EXAMPLE)
    ADD_EXECUTABLE(example example/main.cpp)
    # To add this into global coverage collection:
    #        LINK PRIVATE/PUBLIC and set FLAGS.
    TARGET_LINK_LIBRARIES(example PRIVATE memcov)
    TARGET_COMPILE_OPTIONS(memcov PRIVATE -fsanitize-coverage=trace-pc-guard)
ENDIF()


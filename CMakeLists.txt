CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(memcov)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)


ADD_LIBRARY(memcov-container STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/memcov.cpp)

# Define the coverage behavior.
ADD_LIBRARY(memcov STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/trace.cpp)
TARGET_LINK_LIBRARIES(memcov memcov-container)
TARGET_COMPILE_OPTIONS(memcov PUBLIC -fsanitize-coverage=trace-pc-guard)

# Example code.
ADD_EXECUTABLE(example example/main.cpp)
TARGET_LINK_LIBRARIES(example memcov)

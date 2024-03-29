CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(memcov)

OPTION(MEMCOV_WITH_EXAMPLE OFF)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

# Define the coverage behavior.
ADD_LIBRARY(memcov SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/trace.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/memcov.cpp)
set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/trace.cpp 
    PROPERTIES COMPILE_OPTIONS
    -fsanitize-coverage=edge,trace-pc-guard)

TARGET_INCLUDE_DIRECTORIES(memcov
                           PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
INSTALL(TARGETS  memcov
    EXPORT memcovTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
INSTALL(EXPORT memcovTargets
        FILE memcovTargets.cmake
        NAMESPACE memcov::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/memcovTargets
)

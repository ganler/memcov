CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(memcov)

OPTION(MEMCOV_TEST OFF)

IF(NOT CMAKE_C_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    MESSAGE(FATAL_ERROR "MemCov only works with clang!")
ENDIF()


# Define the coverage behavior.
ADD_LIBRARY(memcov SHARED 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/trace.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/memcov.cpp)

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

IF (MEMCOV_TEST)
    ENABLE_TESTING()
    FILE(GLOB_RECURSE MEMCOV_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.c*)
    FOREACH(TEST_FULL_PATH ${MEMCOV_TESTS})
        MESSAGE(STATUS ">>> [TEST] TO BUILD ${TEST_FULL_PATH}")
        GET_FILENAME_COMPONENT(TEST_NAME ${TEST_FULL_PATH} NAME_WE) 
        # ~ NAME_WE means filename without directory | longest extension
        # ~ See more details at https://cmake.org/cmake/help/v3.0/command/get_filename_component.html

        SET(TEST_TAR ${TEST_NAME}.test)
        ADD_EXECUTABLE(${TEST_TAR} ${TEST_FULL_PATH})
        TARGET_LINK_LIBRARIES(${TEST_TAR} PRIVATE memcov)
        SET_PROPERTY(
            TARGET ${TEST_TAR}
            PROPERTY COMPILE_OPTIONS
            -fsanitize-coverage=bb,no-prune,trace-pc-guard
        )
        ADD_TEST(NAME ${TEST_TAR} COMMAND ${TEST_TAR})
    ENDFOREACH()
ENDIF()
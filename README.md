Quickly allow you to monitor/control in-memory coverage for your C++-implemented-Python-binded libraries.

## **\[C++\]** Add Coverage Tracing During Library Compilation

```cmake
INCLUDE(FetchContent)
FetchContent_Declare(
    memcov
    GIT_REPOSITORY https://github.com/ganler/memcov.git
    GIT_TAG        origin/bitmap
)

FetchContent_GetProperties(memcov)

if(NOT memcov_POPULATED)
    FetchContent_Populate(memcov)
    ADD_SUBDIRECTORY(${memcov_SOURCE_DIR} ${memcov_BINARY_DIR})
endif()

# Allow coverage tracing to specific files!
set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/path/to/what.cpp 
    PROPERTIES COMPILE_OPTIONS
    -fsanitize-coverage=edge,trace-pc-guard)

# Include it to your target!
TARGET_LINK_LIBRARIES(YOUR_TARGET PRIVATE memcov)
```

## **\[Python\]** Add Interface to Operate/Monitor Coverage

```python
import ctypes

_LIB = ctypes.CDLL('/PATH/TO/YOUR_LIB.so')

# calling `reset`, the coverage will not be ZERO (but very small, e.g., 6).
reset = _LIB.mcov_reset

push = _LIB.mcov_push_coverage
pop = _LIB.mcov_pop_coverage

get_total = _LIB.mcov_get_total
get_now = _LIB.mcov_get_now

set_now = _LIB.mcov_set_now

_char_array = ctypes.c_char * get_total()


def get_hitmap():
    hitmap_buffer = bytearray(get_total())
    _LIB.mcov_copy_hitmap(_char_array.from_buffer(hitmap_buffer))
    return hitmap_buffer


def set_hitmap(data):
    assert len(data) == get_total()
    _LIB.mcov_set_hitmap(_char_array.from_buffer(data))
```
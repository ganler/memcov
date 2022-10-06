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

First use this code in your codebase (will be made a Python package soon).

```python
import ctypes


class Memcov:
    def __init__(self, mcov_lib: ctypes.CDLL):
        self.get_hitbits = mcov_lib.mcov_get_hitbits
        self.set_hitbits = mcov_lib.mcov_set_hitbits
        self.reset_bitmap = mcov_lib.mcov_reset_bitmap
        self.get_bitmap_bytes = mcov_lib.mcov_get_bitmap_bytes

        self._char_array = ctypes.c_char * self.get_bitmap_bytes()
        self._mcov_copy_bitmap = mcov_lib.mcov_copy_bitmap
        self._mcov_set_bitmap = mcov_lib.mcov_set_bitmap

    def get_hitmap_buffer(self) -> bytes:
        hitmap_buffer = bytearray(self.get_bitmap_bytes())
        self._mcov_copy_bitmap(self._char_array.from_buffer(hitmap_buffer))
        return hitmap_buffer

    def set_hitmap_buffer(self, data: bytes):
        assert len(data) == self.get_bitmap_bytes()
        self._mcov_set_bitmap(self._char_array.from_buffer(data))
```

## Example: coverage-driven mutation-based fuzzing

```python
cov = Memcov("/PATH/TO/THE/.SO")

last_hits = cov.get_hitbits()
# ... some tests.
if last_hits != cov.get_hitbits(): # coverage changed.
    pass # ... do some mutation...
```

/*
 * Use C codes here please.
 * Introducing many standard or even 3rd-party libraries will mess the tracing
 * part.
 * `///` comment by Jiawei.
 */

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <sanitizer/coverage_interface.h>

#include "coverage_struct.hpp"

constexpr uint32_t kMaxMapSizeInBits = 1 << 16; // See ValueMap in libFuzzer.

/// Called when entering a module (something like Compilation Unit).
/// By setting the index to `0` may stop tracing related BasicBlock at runtime.
// This callback is inserted by the compiler as a module constructor
// into every DSO. 'start' and 'stop' correspond to the
// beginning and end of the section with the guards for the entire
// binary (executable or DSO). The callback will be called at least
// once per DSO and may be called multiple times with the same parameters.
extern "C" void __sanitizer_cov_trace_pc_guard_init(uint32_t* start,
    uint32_t* stop)
{
    if (start == stop || *start)
        return; // Initialize only once.
    /// managed to be compatible with multiple DSOs.
    const uint32_t ncov_dso = stop - start;
    for (uint32_t* x = start; x < stop; x++)
        *x = mcov_total++; // Guards should start from 1.

    // By default, coverage sanitizer enables edge coverage.
    printf("[MCOV::INIT] #CFG edges: {this DSO}: %d and {total}: %d\n",
        ncov_dso, mcov_total);

    if (mcov_bitmap.byte_size * 8 < kMaxMapSizeInBits) {
        const uint32_t available_bits = mcov_bitmap.byte_size * 8;
        const uint32_t required_bits = std::min(mcov_total, kMaxMapSizeInBits);
        const uint32_t bits2alloc = (required_bits > available_bits) ? required_bits - available_bits : 0;
        const size_t bytes2alloc = (bits2alloc + 7) / 8;
        if (bytes2alloc > 0) {
            printf("[MCOV::INIT] Allocating %zu bytes for bitmap", bytes2alloc);
            mcov_bitmap.byte_size += bytes2alloc;
            mcov_bitmap.bytes = (uint8_t*)std::realloc(mcov_bitmap.bytes, mcov_bitmap.byte_size);
            if (nullptr == mcov_bitmap.bytes) {
                printf("[MCOV::INIT] Failed to allocate memory for bitmap");
                std::exit(1);
            }
        }
    }
}

// This callback is inserted by the compiler on every edge in the
// control flow (some optimizations apply).
// Typically, the compiler will emit the code like this:
//    if(*guard)
//      __sanitizer_cov_trace_pc_guard(guard);
// But for large functions it will emit a simple call:
//    __sanitizer_cov_trace_pc_guard(guard);
extern "C" void __sanitizer_cov_trace_pc_guard(uint32_t* guard)
{
    if (!*guard)
        return; // Duplicate the guard check.

    const uint32_t idx = (*guard) % (mcov_bitmap.byte_size * 8);
    const uint32_t byte_idx = idx / 8;
    const uint8_t old_byte = mcov_bitmap.bytes[byte_idx];
    const uint32_t bit_idx = idx % 8;
    const uint8_t new_byte = old_byte | (1 << bit_idx);
    mcov_bitmap.bytes[byte_idx] = new_byte;
    mcov_bitmap.hitbits += (old_byte != new_byte);
}

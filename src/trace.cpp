/*
 * Use C codes here please.
 * Introducing many standard or even 3rd-party libraries will mess the tracing part.
 * `///` comment by Jiawei.
 */

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <sanitizer/coverage_interface.h>

#include "coverage_struct.hpp"

/// Called when entering a module (something like Compilation Unit).
/// By setting the index to `0` may stop tracing related BasicBlock at runtime.
// This callback is inserted by the compiler as a module constructor
// into every DSO. 'start' and 'stop' correspond to the
// beginning and end of the section with the guards for the entire
// binary (executable or DSO). The callback will be called at least
// once per DSO and may be called multiple times with the same parameters.
extern "C" void __sanitizer_cov_trace_pc_guard_init(uint32_t *start, uint32_t *stop) {
    if (start == stop || *start) return;  // Initialize only once.
    /// managed to be compatible with multiple DSOs.
    uint32_t prev_total = mcov_total;
    for (uint32_t *x = start; x < stop; x++)
        *x = ++mcov_total;  // Guards should start from 1.
    // By default, coverage sanitizer enables edge coverage.
    printf("> INIT:: # CFG edge in this DSO: %d; # CFG edge total: %d\n", mcov_total - prev_total, mcov_total);
    mem_coverage.storage_size = (mcov_total + 7) / 8;
    mem_coverage.storage = (uint8_t*)calloc(mem_coverage.storage_size, sizeof(uint8_t));
}

// This callback is inserted by the compiler on every edge in the
// control flow (some optimizations apply).
// Typically, the compiler will emit the code like this:
//    if(*guard)
//      __sanitizer_cov_trace_pc_guard(guard);
// But for large functions it will emit a simple call:
//    __sanitizer_cov_trace_pc_guard(guard);
extern "C" void __sanitizer_cov_trace_pc_guard(uint32_t *guard) {
    if (!*guard) return;  // Duplicate the guard check.
    const uint32_t idx = *guard;
    const uint32_t bit_idx = idx % 8;
    uint8_t& target_byte = mem_coverage.storage[idx / 8];
    if (~(target_byte >> bit_idx) & 0x1) {
        target_byte |= (1 << bit_idx);
        ++mem_coverage.now;
    }
}

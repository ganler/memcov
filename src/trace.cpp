/*
 * Use C codes here please.
 * Introducing many standard or even 3rd-party libraries will mess the tracing part.
 * `///` comment by Jiawei.
 */
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <sanitizer/coverage_interface.h>

#include "../include/memcov.hpp"
#include "coverage_struct.hpp"

/// Called when entering a module (something like Compilation Unit).
/// (*start) contains an index to each basic block (index starting from 1).
/// By setting the index to `0` may stop tracing related BasicBlock at runtime.
// This callback is inserted by the compiler as a module constructor
// into every DSO. 'start' and 'stop' correspond to the
// beginning and end of the section with the guards for the entire
// binary (executable or DSO). The callback will be called at least
// once per DSO and may be called multiple times with the same parameters.
extern "C" void __sanitizer_cov_trace_pc_guard_init(uint32_t *start, uint32_t *stop) {
    using namespace memcov;
    if (start == stop || *start) return;  // Initialize only once.
    /// managed to be compatible with multiple DSOs.
    uint32_t prev_total = coverage.total;
    for (uint32_t *x = start; x < stop; x++)
        *x = ++coverage.total;  // Guards should start from 1.
    printf("> INIT:: # BB in this DSO: %d; # BB total: %d\n", coverage.total - prev_total, coverage.total);
    coverage.visited_bb = (bool*)calloc(coverage.total, sizeof(bool));
}

/// Called per BasicBlock.
// This callback is inserted by the compiler on every edge in the
// control flow (some optimizations apply).
// Typically, the compiler will emit the code like this:
//    if(*guard)
//      __sanitizer_cov_trace_pc_guard(guard);
// But for large functions it will emit a simple call:
//    __sanitizer_cov_trace_pc_guard(guard);
extern "C" void __sanitizer_cov_trace_pc_guard(uint32_t *guard) {
    using namespace memcov;
    if (!*guard) return;  // Duplicate the guard check.
    if (!coverage.visited_bb[*guard]) {
        coverage.visited_bb[*guard] = true;
        ++coverage.now;
    }
}

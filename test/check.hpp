#include <cassert>
#include <cstdio>

#include "../src/coverage_struct.hpp"

__attribute__((no_sanitize("coverage"))) void check_total_cov(uint32_t n)
{
    assert(mcov_total == n);
}

__attribute__((no_sanitize("coverage"))) void check_hitbits(uint32_t n)
{
    assert(mcov_bitmap.hitbits == n);
}

__attribute__((no_sanitize("coverage"))) void check_bitmap_bytes(uint32_t n)
{
    assert(mcov_bitmap.byte_size == n);
}
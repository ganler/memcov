#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "coverage_struct.hpp"

bitmap_t mcov_bitmap;
uint32_t mcov_total = 0;

extern "C" {

void mcov_set_hitbits(uint32_t n) noexcept { mcov_bitmap.hitbits = n; }

void mcov_copy_bitmap(char* ptr) noexcept
{
    std::memcpy(ptr, mcov_bitmap.bytes, mcov_bitmap.byte_size);
}

void mcov_set_bitmap(char* ptr) noexcept
{
    std::memcpy(mcov_bitmap.bytes, ptr, mcov_bitmap.byte_size);
}

uint32_t mcov_hitbits() noexcept { return mcov_bitmap.hitbits; }

void mcov_reset() noexcept
{
    mcov_bitmap.hitbits = 0;
    std::memset(mcov_bitmap.bytes, 0, mcov_bitmap.byte_size);
}
}

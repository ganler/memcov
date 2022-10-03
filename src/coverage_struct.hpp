#pragma once

// NOTE: Try not to include other functions.
#include <cstdint>

struct bitmap_t {
    uint32_t hitbits = 0;
    uint32_t byte_size = 0;
    uint8_t* bytes = nullptr;
};

extern "C" bitmap_t mcov_bitmap;
extern "C" uint32_t mcov_total;

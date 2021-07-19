#pragma once

// NOTE: Try not to include other functions.
#include <cstdint>

struct mcov_t {
    uint32_t now = 0;
    uint32_t storage_size = 0;
    uint8_t* storage = nullptr;
};

extern "C" mcov_t mem_coverage;
extern "C" uint32_t mcov_total;

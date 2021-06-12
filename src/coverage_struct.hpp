#pragma once

namespace memcov {

// NOTE: Try not to include other functions.
struct coverage_container {
    uint32_t total = 0;
    uint32_t now = 0;
    uint8_t* visited_bb = nullptr;
};

inline coverage_container coverage;

}
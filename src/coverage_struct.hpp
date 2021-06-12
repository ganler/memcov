#pragma once

// NOTE: Try not to include other functions.
struct coverage_container {
    uint32_t total = 0;
    uint32_t now = 0;
    char* visited_bb = nullptr;
};

extern "C" coverage_container mem_coverage;

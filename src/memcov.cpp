#include "../include/memcov.hpp"
#include "coverage_struct.hpp"

namespace memcov {

void set_now(uint32_t n) noexcept {
    coverage.now = n;

}

void set_total(uint32_t t) noexcept {
    coverage.total = t;
}

char* ptr() noexcept {
    return coverage.visited_bb;
}

uint32_t get_now() noexcept {
    return coverage.now;
}

uint32_t get_total() noexcept {
    return coverage.total;
}

void reset() noexcept {
    coverage.now = 0;
    for (int i = 0; i < coverage.total; ++i) {
        coverage.visited_bb[i] = false;
    }
}

}
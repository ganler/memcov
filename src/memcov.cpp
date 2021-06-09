#include "../include/memcov.hpp"
#include "coverage_struct.hpp"

namespace memcov {

uint32_t now() noexcept {
    return coverage.now;
}

uint32_t total() noexcept {
    return coverage.total;
}

void reset() noexcept {
    coverage.now = 0;
    for (int i = 0; i < coverage.total; ++i) {
        coverage.visited_bb[i] = false;
    }
}

}
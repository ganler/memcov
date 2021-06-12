#include <cstdint>
#include "coverage_struct.hpp"

extern "C" {

coverage_container mem_coverage;

void mcov_set_now(uint32_t n) noexcept {
    mem_coverage.now = n;
}

void mcov_set_total(uint32_t t) noexcept {
    mem_coverage.total = t;
}

char* mcov_ptr() noexcept {
    return mem_coverage.visited_bb;
}

uint32_t mcov_get_now() noexcept {
    return mem_coverage.now;
}

uint32_t mcov_get_total() noexcept {
    return mem_coverage.total;
}

void mcov_reset() noexcept {
    mem_coverage.now = 0;
    for (int i = 0; i < mem_coverage.total; ++i) {
        mem_coverage.visited_bb[i] = false;
    }
}

}

#include <_types/_uint32_t.h>
#include <cstdint>
#include <cstring>

#include "coverage_struct.hpp"

extern "C" {

mcov_t mem_coverage;

void mcov_set_now(uint32_t n) noexcept {
    mem_coverage.now = n;
}

void mcov_set_total(uint32_t t) noexcept {
    mem_coverage.total = t;
}

void mcov_copy_hitmap(char* ptr) noexcept {
    std::memcpy(ptr, mem_coverage.storage, mem_coverage.storage_size);
}

void mcov_set_hitmap(char* ptr) noexcept {
    std::memcpy(mem_coverage.storage, ptr, mem_coverage.storage_size);
}

uint32_t mcov_get_now() noexcept {
    return mem_coverage.now;
}

uint32_t mcov_get_total() noexcept {
    return mem_coverage.total;
}

void mcov_reset() noexcept {
    mem_coverage.now = 0;
    std::memset(mem_coverage.storage, 0, mem_coverage.storage_size);
}

}

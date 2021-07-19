#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <utility>

#include "coverage_struct.hpp"

mcov_t mem_coverage;
uint32_t mcov_total;

constexpr size_t mcov_max_stack_size = 8;
static mcov_t mcov_stack[mcov_max_stack_size];
static size_t mcov_stack_size = 0;

static mcov_t mcov_make_coverage() noexcept {
    mcov_t new_cov;
    new_cov.storage_size = (mcov_total + 7) / 8;
    new_cov.storage = (uint8_t*)std::malloc(mem_coverage.storage_size * sizeof(uint8_t));
    std::memset(new_cov.storage, 0, new_cov.storage_size);
    return new_cov;
}

extern "C" {

// Reset current coverage and push it to a stack.
void mcov_push_coverage() noexcept {
    if (mcov_stack_size == mcov_max_stack_size) {
        printf("mcov_push_coverage: stack overflow! max coverage stack size is %li\n", mcov_max_stack_size);
        abort();
    }
    mcov_stack[mcov_stack_size] = mcov_make_coverage();
    std::swap(mcov_stack[mcov_stack_size], mem_coverage);
    ++mcov_stack_size;
}

// Pop last coverage from the stack and merge it into current coverage.
void mcov_pop_coverage() noexcept {
    if (0 == mcov_stack_size) {
        printf("You cannot pop an empty stack!");
        abort();
    }

    mcov_t last_cov = mcov_stack[mcov_stack_size - 1];
    uint32_t count = 0;
    // merge it into current cov.
    for (size_t i = 0; i < mem_coverage.storage_size; ++i) {
        mem_coverage.storage[i] |= last_cov.storage[i];
        count += __builtin_popcount(mem_coverage.storage[i]);
    }
    mem_coverage.now = count;
    free(last_cov.storage);
    --mcov_stack_size;
}

void mcov_set_now(uint32_t n) noexcept {
    mem_coverage.now = n;
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
    return mcov_total;
}

void mcov_reset() noexcept {
    mem_coverage.now = 0;
    std::memset(mem_coverage.storage, 0, mem_coverage.storage_size);
}

}

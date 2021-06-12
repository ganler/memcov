//
// Created by ganler-Mac on 2021-06-09.
//

#pragma once

#include <cstdint>

namespace memcov {

char* ptr() noexcept;
void reset() noexcept;

uint32_t get_now() noexcept;
uint32_t get_total() noexcept;

void set_now(uint32_t) noexcept;
void set_total(uint32_t) noexcept;

}

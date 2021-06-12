//
// Created by ganler-Mac on 2021-06-09.
//

#pragma once

#include <cstdint>

namespace memcov {

const uint8_t* ptr() noexcept;
void reset() noexcept;
uint32_t now() noexcept;
uint32_t total() noexcept;

}

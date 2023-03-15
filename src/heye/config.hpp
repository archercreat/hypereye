#pragma once

static constexpr auto max_cpu_count     = 256;
static constexpr auto pool_tag          = 'heye';
static constexpr auto page_size         = 0x1000;
static constexpr auto page_shift        = 12;
static constexpr auto kernel_stack_size = 6 * page_size;

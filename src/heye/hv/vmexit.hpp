#pragma once

#include "vmx.hpp"

#include <cstdint>

namespace heye
{
struct vcpu_t;

namespace vmexit
{
bool passthrough(vcpu_t* vcpu);
};
};

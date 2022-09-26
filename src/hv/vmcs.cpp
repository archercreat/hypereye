#include "vmcs.hpp"
#include "arch/memory.hpp"

namespace vmx
{
uint64_t vmcs_t::pa()
{
    return pa_from_va(this);
}
};

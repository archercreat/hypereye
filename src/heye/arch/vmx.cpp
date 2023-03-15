#include "vmx.hpp"
#include "asm.hpp"

namespace heye::vmx
{
void invept(vmx::invept_t type, uint64_t eptp)
{
    vmx::invept_desc_t descriptor{ .eptp = eptp };
    asm_invept(static_cast<uint64_t>(type), &descriptor);
}

void invvpid(vmx::invvpid_t type, uint64_t vpid, uint64_t address)
{
    vmx::invvpid_desc_t descriptor{};
    switch (type)
    {
    case vmx::invvpid_t::linear_address:
        descriptor.vpid           = vpid;
        descriptor.linear_address = address;
        break;
    case vmx::invvpid_t::single_context:
        descriptor.vpid = vpid;
        break;
    default:
        break;
    }
    asm_invvpid(static_cast<uint64_t>(type), &descriptor);
}
} // namespace heye::vmx

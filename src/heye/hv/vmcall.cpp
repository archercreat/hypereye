#include "vmcall.hpp"
#include "vcpu.hpp"
#include "vmx.hpp"

#include "heye/arch/arch.hpp"

#include "heye/shared/trace.hpp"
#include "heye/shared/cpu.hpp"

namespace heye
{
bool handle_vmcall(vcpu_t* vcpu)
{
    auto reason = static_cast<vmcall_reason>(vcpu->regs().rcx);

    switch (reason)
    {
    case vmcall_reason::ping:
    {
        logger::info("pong :)");
        break;
    }
    case vmcall_reason::vmxoff:
    {
        logger::info("vmxoff called");
        // Set rcx to the next instruction address and rdx to the guest stack pointer.
        //
        vcpu->regs().rip = read<vmx::vmcs::guest_rip>() + read<vmx::vmcs::vm_exit_instruction_len>();
        // Since we will not vmresume, we must overwrite host cr3 with the guest cr3.
        //
        write<cr3_t> (cr3_t{ read<vmx::vmcs::guest_cr3>() });
        write<gdtr_t>(gdtr_t{ static_cast<uint16_t>(read<vmx::vmcs::guest_gdtr_limit>() & 0xffff), read<vmx::vmcs::guest_gdtr_base>() });
        write<idtr_t>(idtr_t{ static_cast<uint16_t>(read<vmx::vmcs::guest_idtr_limit>() & 0xffff), read<vmx::vmcs::guest_idtr_base>() });
        return true;
    }
    default:
        break;
    }
    return false;
}
};

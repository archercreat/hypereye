#include "vmcall.hpp"
#include "vmx.hpp"

#include "heye/arch/arch.hpp"

#include "heye/shared/trace.hpp"
#include "heye/shared/cpu.hpp"

bool handle_vmcall(cpu::context_t* context)
{
    bool terminate    = false;
    const auto reason = static_cast<vmcall_reason>(context->rcx);

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
        context->rax = 1;
        context->rcx = vmx::read(vmx::vmcs::guest_rip) + vmx::read(vmx::vmcs::vm_exit_instruction_len);
        context->rdx = vmx::read(vmx::vmcs::guest_rsp);
        // Since we will not vmresume, we must overwrite host cr3 with the guest cr3.
        //
        write<cr3_t> (cr3_t{ vmx::read(vmx::vmcs::guest_cr3) });
        write<gdtr_t>(gdtr_t{ static_cast<uint16_t>(vmx::read(vmx::vmcs::guest_gdtr_limit) & 0xffff), vmx::read(vmx::vmcs::guest_gdtr_base) });
        write<idtr_t>(idtr_t{ static_cast<uint16_t>(vmx::read(vmx::vmcs::guest_idtr_limit) & 0xffff), vmx::read(vmx::vmcs::guest_idtr_base) });

        terminate = true;
        break;
    }
    default:
        break;
    }
    return terminate;
}

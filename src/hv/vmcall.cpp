#include "vmcall.hpp"
#include "vmx.hpp"

#include "arch/arch.hpp"

#include "shared/trace.hpp"
#include "shared/cpu.hpp"

bool handle_vmcall(cpu::context_t* context)
{
    bool terminate    = false;
    const auto reason = static_cast<vmcall_reason>(context->rcx);

    switch (reason)
    {
    case vmcall_reason::hello:
    {
        logger::info("VMCALL: hello from hypervisor :)");
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
        write<cr3_t> ({ vmx::read(vmx::vmcs::guest_cr3) });
        write<gdtr_t>({ vmx::read(vmx::vmcs::guest_gdtr_limit) & 0xffff, vmx::read(vmx::vmcs::guest_gdtr_base) });
        write<idtr_t>({ vmx::read(vmx::vmcs::guest_idtr_limit) & 0xffff, vmx::read(vmx::vmcs::guest_idtr_base) });

        terminate = true;
        break;
    }
    default:
        break;
    }
    return terminate;
}

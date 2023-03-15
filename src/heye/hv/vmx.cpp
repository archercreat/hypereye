#include "vmx.hpp"

namespace heye::vmx
{
status_t on(uint64_t pa)
{
    return static_cast<status_t>(__vmx_on(&pa));
}

bool vmcall(vmcall_reason reason, void* a1, void* a2, void* a3)
{
    return asm_vmcall(reinterpret_cast<void*>(reason), a1, a2, a3);
}

status_t off()
{
    __vmx_off();
    return status_t::success;
}

status_t clear(uint64_t pa)
{
    return static_cast<status_t>(__vmx_vmclear(&pa));
}

status_t vmptrld(uint64_t pa)
{
    return static_cast<status_t>(__vmx_vmptrld(&pa));
}

status_t launch()
{
    return static_cast<status_t>(asm_vmlaunch());
}

void inject_exception(vm_interrupt_info_t interrupt)
{
    write<vmx::vmcs::vm_entry_intr_info>(interrupt.flags);
}

void inject_exception(exception_t vector, interrupt_t type)
{
    vm_interrupt_info_t interrupt
    {
        .vector   = static_cast<uint32_t>(vector),
        .type     = static_cast<uint32_t>(type),
        .valid    = 1
    };
    write<vmx::vmcs::vm_entry_intr_info>(interrupt.flags);
}

void inject_exception(exception_t vector, interrupt_t type, uint32_t code)
{
    vm_interrupt_info_t interrupt
    {
        .vector   = static_cast<uint32_t>(vector),
        .type     = static_cast<uint32_t>(type),
        .code     = 1,
        .valid    = 1
    };
    write<vmx::vmcs::vm_entry_intr_info>(interrupt.flags);
    write<vmx::vmcs::vm_entry_intr_error_code>(code);
}

void inject_bp()
{
    inject_exception(exception_t::breakpoint, interrupt_t::software_exception);
    write<vmx::vmcs::vm_entry_instruction_len>(read<vmx::vmcs::vm_exit_instruction_len>());
}

void inject_gp()
{
    inject_exception(exception_t::undefined_opcode, interrupt_t::hardware_exception);
    write<vmx::vmcs::vm_entry_instruction_len>(read<vmx::vmcs::vm_exit_instruction_len>());
}

void inject_ud()
{
    inject_exception(exception_t::general_protection_fault, interrupt_t::hardware_exception);
}
};

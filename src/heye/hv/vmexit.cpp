#include "vmexit.hpp"
#include "vmcall.hpp"
#include "vcpu.hpp"

#include "heye/shared/trace.hpp"
#include "heye/shared/cpu.hpp"
#include "heye/arch/arch.hpp"

#include <ntddk.h>

namespace heye
{
static void handle_exception(vcpu_t* vcpu)
{
    auto interrupt = vcpu->exit_interrupt_info();

    switch (interrupt.type)
    {
    case vmx::interrupt_t::external_interrupt:
        break;

    default:
        break;
    }
    vcpu->skip_instruction();
}

static void handle_cpuid(vcpu_t* vcpu)
{
    // #UD If the LOCK prefix is used.
    //
    int info[4];
    __cpuidex(info, vcpu->regs().eax, vcpu->regs().ecx);
    vcpu->regs().rax = info[0];
    vcpu->regs().rbx = info[1];
    vcpu->regs().rcx = info[2];
    vcpu->regs().rdx = info[3];
    vcpu->skip_instruction();
}

static void handle_invd(vcpu_t* vcpu)
{
    __wbinvd();
    vcpu->skip_instruction();
}

static void handle_invlpg(vcpu_t* vcpu)
{
    auto linear_address = vcpu->exit_qualification().linear_address;
    invvpid(vmx::invvpid_t::linear_address, vcpu->id(), linear_address);
    vcpu->skip_instruction();
}

static void handle_msr_read(vcpu_t* vcpu)
{
    auto id = vcpu->regs().ecx;
    uint64_t value{};

    switch (id)
    {
    case msr::sysenter_cs::id:
        value = read<vmx::vmcs::guest_sysenter_cs>();
        break;
    case msr::sysenter_eip::id:
        value = read<vmx::vmcs::guest_sysenter_eip>();
        break;
    case msr::sysenter_esp::id:
        value = read<vmx::vmcs::guest_sysenter_esp>();
        break;
    case msr::fsbase::id:
        value = read<vmx::vmcs::guest_fs_base>();
        break;
    case msr::gsbase::id:
        value = read<vmx::vmcs::guest_gs_base>();
        break;
    case msr::debugctl::id:
        value = read<vmx::vmcs::guest_ia32_debugctl>();
        break;
    default:
        // TODO: Check for valid msr.
        //
        value = __readmsr(id);
        break;
    }
    vcpu->regs().rax = (value >>  0) & 0xffffffff;
    vcpu->regs().rdx = (value >> 32) & 0xffffffff;
    vcpu->skip_instruction();
}

static void handle_msr_write(vcpu_t* vcpu)
{
    // TODO: Check for valid msr.
    //
    auto id    = vcpu->regs().ecx;
    auto value = vcpu->regs().rax | vcpu->regs().rdx << 32;

    switch (id)
    {
    case msr::sysenter_cs::id:
        write<msr::sysenter_cs>(msr::sysenter_cs{ value });
        break;
    case msr::sysenter_eip::id:
        write<msr::sysenter_eip>(msr::sysenter_eip{ value });
        break;
    case msr::sysenter_esp::id:
        write<msr::sysenter_esp>(msr::sysenter_esp{ value });
        break;
    case msr::fsbase::id:
        write<msr::fsbase>(msr::fsbase{ value });
        break;
    case msr::gsbase::id:
        write<msr::gsbase>(msr::gsbase{ value });
        break;
    case msr::debugctl::id:
        write<msr::debugctl>(msr::debugctl{ value });
        break;
    default:
        // TODO: Check for valid msr.
        //
        __writemsr(id, value);
        break;
    }
    vcpu->skip_instruction();
}

static void handle_vmx_fallback(vcpu_t*)
{
    // Inject undefined opcode.
    //
    vmx::inject_ud();
}

namespace vmexit
{
bool passthrough(vcpu_t* vcpu)
{
    bool terminate = false;

    switch (vcpu->exit_reason())
    {
    case vmx::exit_reason::exception_nmi:
    {
        handle_exception(vcpu);
        break;
    }
    case vmx::exit_reason::cpuid:
    {
        handle_cpuid(vcpu);
        break;
    }
    case vmx::exit_reason::invd:
    {
        handle_invd(vcpu);
        break;
    }
    case vmx::exit_reason::invlpg:
    {
        handle_invlpg(vcpu);
        break;
    }
    case vmx::exit_reason::vmclear:  [[fallthrough]];
    case vmx::exit_reason::vmptrld:  [[fallthrough]];
    case vmx::exit_reason::vmptrst:  [[fallthrough]];
    case vmx::exit_reason::vmread:   [[fallthrough]];
    case vmx::exit_reason::vmresume: [[fallthrough]];
    case vmx::exit_reason::vmwrite:  [[fallthrough]];
    case vmx::exit_reason::vmxoff:   [[fallthrough]];
    case vmx::exit_reason::vmlaunch:
    {
        handle_vmx_fallback(vcpu);
        break;
    }
    case vmx::exit_reason::ept_violation: [[fallthrough]];
    case vmx::exit_reason::ept_misconfig:
    {
        __debugbreak();
        break;
    }

    case vmx::exit_reason::msr_read:
    {
        handle_msr_read(vcpu);
        break;
    }
    case vmx::exit_reason::msr_write:
    {
        handle_msr_write(vcpu);
        break;
    }
    case vmx::exit_reason::vmcall:
    {
        terminate = handle_vmcall(vcpu);
        break;
    }
    default:
        __debugbreak();
        break;
    }
    return terminate;
}
}; // namespace vmexit
}; // namespace heye

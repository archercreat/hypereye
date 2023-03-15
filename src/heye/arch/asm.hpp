#pragma once

#include <cstdint>

/// Registers controls.
///
extern "C" uint64_t asm_read_dr0();
extern "C" uint64_t asm_read_dr1();
extern "C" uint64_t asm_read_dr2();
extern "C" uint64_t asm_read_dr3();
extern "C" uint64_t asm_read_dr6();
extern "C" uint64_t asm_read_dr7();

extern "C" uint64_t asm_read_rflags();

extern "C" void asm_read_gdtr(void* gdtr);
extern "C" void asm_read_idtr(void* idtr);

extern "C" void asm_write_gdtr(const void* gdtr);
extern "C" void asm_write_idtr(const void* idtr);

/// Selectors.
///
extern "C" uint16_t asm_read_cs();
extern "C" uint16_t asm_read_ds();
extern "C" uint16_t asm_read_es();
extern "C" uint16_t asm_read_ss();
extern "C" uint16_t asm_read_fs();
extern "C" uint16_t asm_read_gs();
extern "C" uint16_t asm_read_tr();
extern "C" uint16_t asm_read_ldtr();

/// Read segment access rights.
///
extern "C" uint64_t asm_lar(uint64_t);

/// VMX invalidate ept.
///
extern "C" uint64_t asm_invept(uint64_t, void*);

/// VMX invalidate vpid.
///
extern "C" uint64_t asm_invvpid(uint64_t, void*);

/// VMX call hypervisor.
///
extern "C" bool asm_vmcall(void*, void*, void*, void*);

/// VMX vm-exit function.
///
extern "C" void vmexit_stub();

/// Launch vm. Return 0 on success meaning cpu executing now
/// in non-root operation. return error code based on __vmx_vmlaunch intrinsic.
///
extern "C" uint8_t asm_vmlaunch();

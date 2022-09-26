#pragma once

/// @brief Registers controls.
///
extern "C" unsigned long long asm_read_dr0();
extern "C" unsigned long long asm_read_dr1();
extern "C" unsigned long long asm_read_dr2();
extern "C" unsigned long long asm_read_dr3();
extern "C" unsigned long long asm_read_dr6();
extern "C" unsigned long long asm_read_dr7();

extern "C" unsigned long long asm_read_rflags();

extern "C" void asm_read_gdtr(void* gdtr);
extern "C" void asm_read_idtr(void* idtr);

extern "C" void asm_write_gdtr(const void* gdtr);
extern "C" void asm_write_idtr(const void* idtr);

/// @brief Selectors.
///
extern "C" unsigned short asm_read_cs();
extern "C" unsigned short asm_read_ds();
extern "C" unsigned short asm_read_es();
extern "C" unsigned short asm_read_ss();
extern "C" unsigned short asm_read_fs();
extern "C" unsigned short asm_read_gs();
extern "C" unsigned short asm_read_tr();
extern "C" unsigned short asm_read_ldtr();

/// @brief
///
extern "C" unsigned long long asm_lar(unsigned long long);

/// @brief VMX invalidate ept.
///
extern "C" unsigned long long asm_invept(unsigned long long, void*);

extern "C" unsigned long long asm_invvpid(unsigned long long, void*);

/// @brief VMX call hypervisor.
///
extern "C" bool asm_vmcall(void*, void*, void*, void*);

/// @brief VMX vm-exit function.
///
extern "C" void vmexit_stub();

/// @brief VMX initialize guest.
///
extern "C" bool vminit_stub(void*, void*);
extern "C" bool vmresume_stub();

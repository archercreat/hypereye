.code

VMX_ERROR_CODE_SUCCESS            = 0
VMX_ERROR_CODE_FAILED_WITH_STATUS = 1
VMX_ERROR_CODE_FAILED             = 2

VMCS_GUEST_RSP     = 0681Ch
VMCS_GUEST_RIP     = 0681Eh
VMCS_GUEST_RFLAGS  = 06820h
KGDT64_R3_DATA     = 00028h
KGDT64_R3_CODE     = 00030h
RPL_MASK           = 00003h
SHADOW_SPACE_SIZE  = 00020h

; Defined in cpu.hpp.
regs_t struct
    ; General-purpose registers.
    $rax    qword ?
    $rcx    qword ?
    $rbx    qword ?
    $rdx    qword ?
    $rsp    qword ?
    $rbp    qword ?
    $rsi    qword ?
    $rdi    qword ?
    $r8     qword ?
    $r9     qword ?
    $r10    qword ?
    $r11    qword ?
    $r12    qword ?
    $r13    qword ?
    $r14    qword ?
    $r15    qword ?
    $rip    qword ?
    $rflags qword ?
    ; SSE registers.
    $xmm0   oword ?
    $xmm1   oword ?
    $xmm2   oword ?
    $xmm3   oword ?
    $xmm4   oword ?
    $xmm5   oword ?
    $xmm6   oword ?
    $xmm7   oword ?
    $xmm8   oword ?
    $xmm9   oword ?
    $xmm10  oword ?
    $xmm11  oword ?
    $xmm12  oword ?
    $xmm13  oword ?
    $xmm14  oword ?
    $xmm15  oword ?
regs_t ends

; Defined in vcpu.hpp
frame_t struct
    $rip    qword ?
    $cs     qword ?
    $rflags qword ?
    $rsp    qword ?
    $ss     qword ?
frame_t ends

vmexit_stub proc frame
    ; Create trap frame for stack traces.
    sub    rsp, 8 + sizeof frame_t
    push   rax
    mov    frame_t.$ss[rsp + 8], KGDT64_R3_DATA or RPL_MASK
    mov    rax, VMCS_GUEST_RSP
    vmread frame_t.$rsp[rsp + 8], rax
    mov    rax, VMCS_GUEST_RFLAGS
    vmread frame_t.$rflags[rsp + 8], rax
    mov    frame_t.$cs[rsp + 8], KGDT64_R3_CODE or RPL_MASK
    mov    rax, VMCS_GUEST_RIP
    vmread frame_t.$rip[rsp + 8], rax
    pop    rax
    .pushframe

    sub    rsp, sizeof regs_t
    .allocstack sizeof regs_t + SHADOW_SPACE_SIZE
    .endprolog

    ; General-purpose registers.
    mov    regs_t.$rax[rsp], rax
    mov    regs_t.$rcx[rsp], rcx
    mov    regs_t.$rbx[rsp], rbx
    mov    regs_t.$rdx[rsp], rdx
    mov    regs_t.$rsp[rsp], rsp
    mov    regs_t.$rbp[rsp], rbp
    mov    regs_t.$rsi[rsp], rsi
    mov    regs_t.$rdi[rsp], rdi
    mov    regs_t.$r8[rsp],  r8
    mov    regs_t.$r9[rsp],  r9
    mov    regs_t.$r10[rsp], r10
    mov    regs_t.$r11[rsp], r11
    mov    regs_t.$r12[rsp], r12
    mov    regs_t.$r13[rsp], r13
    mov    regs_t.$r14[rsp], r14
    mov    regs_t.$r15[rsp], r15

    mov    rax, VMCS_GUEST_RSP
    vmread regs_t.$rsp[rsp], rax
    mov    rax, VMCS_GUEST_RIP
    vmread regs_t.$rip[rsp], rax
    mov    rax, VMCS_GUEST_RFLAGS
    vmread regs_t.$rflags[rsp], rax
    ; SSE registers.
    movaps regs_t.$xmm0[rsp],  xmm0
    movaps regs_t.$xmm1[rsp],  xmm1
    movaps regs_t.$xmm2[rsp],  xmm2
    movaps regs_t.$xmm3[rsp],  xmm3
    movaps regs_t.$xmm4[rsp],  xmm4
    movaps regs_t.$xmm5[rsp],  xmm5
    movaps regs_t.$xmm6[rsp],  xmm6
    movaps regs_t.$xmm7[rsp],  xmm7
    movaps regs_t.$xmm8[rsp],  xmm8
    movaps regs_t.$xmm9[rsp],  xmm9
    movaps regs_t.$xmm10[rsp], xmm10
    movaps regs_t.$xmm11[rsp], xmm11
    movaps regs_t.$xmm12[rsp], xmm12
    movaps regs_t.$xmm13[rsp], xmm13
    movaps regs_t.$xmm14[rsp], xmm14
    movaps regs_t.$xmm15[rsp], xmm15
    ; Make shadow space.
    sub    rsp, SHADOW_SPACE_SIZE
    ; Load vcpu pointer.
    mov    rcx, [rsp + 30h + sizeof frame_t + sizeof regs_t]
    ; Call vmexit handler.
    call   qword ptr [rsp + 28h + sizeof frame_t + sizeof regs_t]

    add    rsp, SHADOW_SPACE_SIZE

    cmp    al, 1
    je     vmxoff_stub

    vmresume
    jmp    @error

vmxoff_stub:
    movaps xmm0,  regs_t.$xmm0[rsp]
    movaps xmm1,  regs_t.$xmm1[rsp]
    movaps xmm2,  regs_t.$xmm2[rsp]
    movaps xmm3,  regs_t.$xmm3[rsp]
    movaps xmm4,  regs_t.$xmm4[rsp]
    movaps xmm5,  regs_t.$xmm5[rsp]
    movaps xmm6,  regs_t.$xmm6[rsp]
    movaps xmm7,  regs_t.$xmm7[rsp]
    movaps xmm8,  regs_t.$xmm8[rsp]
    movaps xmm9,  regs_t.$xmm9[rsp]
    movaps xmm10, regs_t.$xmm10[rsp]
    movaps xmm11, regs_t.$xmm11[rsp]
    movaps xmm12, regs_t.$xmm12[rsp]
    movaps xmm13, regs_t.$xmm13[rsp]
    movaps xmm14, regs_t.$xmm14[rsp]
    movaps xmm15, regs_t.$xmm15[rsp]

    mov    rax, regs_t.$rax[rsp]
    mov    rcx, regs_t.$rcx[rsp]
    mov    rdx, regs_t.$rdx[rsp]
    mov    rbx, regs_t.$rbx[rsp]
    mov    rbp, regs_t.$rbp[rsp]
    mov    rsi, regs_t.$rsi[rsp]
    mov    rdi, regs_t.$rdi[rsp]
    mov    r8,  regs_t.$r8[rsp]
    mov    r9,  regs_t.$r9[rsp]
    mov    r10, regs_t.$r10[rsp]
    mov    r11, regs_t.$r11[rsp]
    mov    r12, regs_t.$r12[rsp]
    mov    r13, regs_t.$r13[rsp]
    mov    r14, regs_t.$r14[rsp]
    mov    r15, regs_t.$r15[rsp]
    mov    rcx, regs_t.$rip[rsp]
    mov    rdx, regs_t.$rsp[rsp]

    add    rsp, sizeof regs_t

    vmxoff
    jz     @error
    jc     @error

    mov    rsp, rdx
    jmp    rcx

@error:
    int 3
vmexit_stub endp

asm_vmlaunch proc
    mov     rcx, VMCS_GUEST_RSP
    vmwrite rcx, rsp        ; Set guest stack to the current stack.
    jz @jz
    jc @jc

    mov     rcx, VMCS_GUEST_RIP
    lea     rdx, done
    vmwrite rcx, rdx        ; Set guest rip to the `done` label.
    jz @jz
    jc @jc

    pushfq
    pop     rdx
    mov     rcx, VMCS_GUEST_RFLAGS
    vmwrite rcx, rdx        ; Set guest rflags.
    jz @jz
    jc @jc

    vmlaunch                ; Following __vmx_vmlaunch abi.
    jz @jz
    jc @jc

done:
    xor     rax, rax
    ret
@jz:
    mov     rax, VMX_ERROR_CODE_FAILED_WITH_STATUS
    ret
@jc:
    mov     rax, VMX_ERROR_CODE_FAILED
    ret
asm_vmlaunch endp

asm_invept proc
    invept  rcx, oword ptr [rdx]
    jz @jz
    jc @jc
    xor     rax, rax
    ret

@jz:
    mov     rax, VMX_ERROR_CODE_FAILED_WITH_STATUS
    ret

@jc:
    mov     rax, VMX_ERROR_CODE_FAILED
    ret
asm_invept endp

asm_invvpid proc
    invvpid rcx, oword ptr [rdx]
    jz      @jz
    jc      @jc
    xor     rax, rax
    ret
@jz:
    mov     rax, VMX_ERROR_CODE_FAILED_WITH_STATUS
    ret
@jc:
    mov     rax, VMX_ERROR_CODE_FAILED
    ret
asm_invvpid endp

asm_vmcall proc
    vmcall
    ret
asm_vmcall endp
end

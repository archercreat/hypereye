.code

EXTERN vmexit_handler:PROC

VMX_ERROR_CODE_SUCCESS            = 0
VMX_ERROR_CODE_FAILED_WITH_STATUS = 1
VMX_ERROR_CODE_FAILED             = 2

VMCS_GUEST_RSP    = 0681Ch
VMCS_GUEST_RIP    = 0681Eh
VMCS_GUEST_RFLAGS = 06820h

PUSHAQ MACRO
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8     ; +64
    push rdi    ; +56
    push rsi    ; +48
    push rbp    ; +40
    push 0      ; +32
    push rdx    ; +24
    push rcx    ; +16
    push rbx    ; +8
    push rax    ; +0
    mov  [rsp + 32], rsp    ; Save stack value.
ENDM

POPAQ MACRO
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rbp
    pop rbp
    pop rsi
    pop rdi
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
ENDM

vmexit_stub proc
    pushfq
    PUSHAQ

    mov rcx, rsp
    sub rsp, 20h

    call vmexit_handler

    add rsp, 20h

    cmp al, 1
    je vmxoff_stub

    POPAQ
    popfq

    vmresume
    jmp error

vmxoff_stub:
    POPAQ
    popfq

    vmxoff
    jz error
    jc error

    mov rsp, rdx
    jmp rcx

error:
    int 3
vmexit_stub endp

asm_vmlaunch proc
    mov     rcx, VMCS_GUEST_RSP
    vmwrite rcx, rsp        ; Set guest stack to the current stack.
    jz      failure
    jc      failure

    mov     rcx, VMCS_GUEST_RIP
    lea     rdx, done
    vmwrite rcx, rdx        ; Set guest rip to the `done` label.
    jz      failure
    jc      failure

    pushfq
    pop     rdx
    mov     rcx, VMCS_GUEST_RFLAGS
    vmwrite rcx, rdx        ; Set guest rflags.
    jz      failure
    jc      failure

    vmlaunch                ; Following __vmx_vmlaunch abi.
    jz      failure
    jc      failure

done:
    xor     rax, rax
    ret
failure:
    xor     rax, rax
    inc     rax
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

.code

PUBLIC asm_read_dr0
PUBLIC asm_read_dr1
PUBLIC asm_read_dr2
PUBLIC asm_read_dr3
PUBLIC asm_read_dr6
PUBLIC asm_read_dr7

PUBLIC asm_read_rflags

PUBLIC asm_read_gdtr
PUBLIC asm_read_idtr
PUBLIC asm_read_ldtr
PUBLIC asm_read_tr

PUBLIC asm_read_cs
PUBLIC asm_read_ds
PUBLIC asm_read_es
PUBLIC asm_read_ss
PUBLIC asm_read_fs
PUBLIC asm_read_gs

PUBLIC asm_write_gdtr
PUBLIC asm_write_idtr

asm_read_dr0 proc
    mov rax, dr0
    ret
asm_read_dr0 endp

asm_read_dr1 proc
    mov rax, dr1
    ret
asm_read_dr1 endp

asm_read_dr2 proc
    mov rax, dr2
    ret
asm_read_dr2 endp

asm_read_dr3 proc
    mov rax, dr3
    ret
asm_read_dr3 endp

asm_read_dr6 proc
    mov rax, dr6
    ret
asm_read_dr6 endp

asm_read_dr7 proc
    mov rax, dr7
    ret
asm_read_dr7 endp

asm_read_rflags proc
    pushfq
    pop rax
    ret
asm_read_rflags endp

asm_read_gdtr proc
    sgdt fword ptr [rcx]
    ret
asm_read_gdtr endp

asm_read_idtr proc
    sidt fword ptr [rcx]
    ret
asm_read_idtr endp

asm_read_ldtr proc
    sldt ax
    ret
asm_read_ldtr endp

asm_read_tr proc
    str ax
    ret
asm_read_tr endp

asm_read_cs proc
    mov ax, cs
    ret
asm_read_cs endp

asm_read_ds proc
    mov ax, ds
    ret
asm_read_ds endp

asm_read_es proc
    mov ax, es
    ret
asm_read_es endp

asm_read_ss proc
    mov ax, ss
    ret
asm_read_ss endp

asm_read_fs proc
    mov ax, fs
    ret
asm_read_fs endp

asm_read_gs proc
    mov ax, gs
    ret
asm_read_gs endp

asm_lar proc
    lar rax, rcx
    ret
asm_lar endp

asm_write_gdtr proc
    lgdt fword ptr [rcx]
    ret
asm_write_gdtr endp

asm_write_idtr proc
    lidt fword ptr [rcx]
    ret
asm_write_idtr endp

end

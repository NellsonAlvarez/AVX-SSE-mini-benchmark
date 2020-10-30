section .text

%macro prologue 1

    push rbp
    mov rbp, rsp
    %if %1 = 1
        vmovdqa xmm0, oword [rdi]
        vmovdqa xmm1, oword [rsi]
    %elif %1 = 2
        vmovdqa xmm1, oword [rdi]
        vmovdqa xmm2, oword [rsi]
    %endif

%endmacro

global dot_prod_avx_p
dot_prod_avx_p:
    prologue 1

    vmulps xmm0,xmm0, xmm1
    vhaddps xmm0,xmm0,xmm3
    vhaddps xmm0,xmm0,xmm3

    leave
    ret

global dot_prod_avx_s
dot_prod_avx_s:
    prologue 2

    push rax
    push rcx
    
    mov rax, 0
    mov rcx, 4

    cicloAVX:
        vmovd xmm1, [rdi+rax]
        vmovd xmm2, [rsi+rax]
        vmulss xmm1,xmm1, xmm2
        vaddss xmm0,xmm0,xmm1
        add rax, 4
    loop cicloAVX

    movd eax, xmm0

    pop rax
    pop rcx

    leave
    ret

global dot_prod_sse_p
dot_prod_sse_p:
    prologue 1

    mulps xmm0, xmm1
    haddps xmm0,xmm3
    haddps xmm0,xmm3

    leave
    ret

global dot_prod_sse_s
dot_prod_sse_s:
    prologue 2
    push rax
    push rcx

    mov rax, 0
    mov rcx, 4

    cicloSSE:
        movd xmm1, [rdi+rax]
        movd xmm2, [rsi+rax]
        mulss xmm1, xmm2
        addss xmm0,xmm1
        add rax, 4
    loop cicloSSE
    
    pop rax
    pop rcx

    leave
    ret
 



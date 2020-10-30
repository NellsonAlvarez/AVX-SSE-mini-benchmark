; Nelson Alvarez
; Dean Vargas

section .text

%macro prologue 1

    push rbp
    mov rbp, rsp
    %if %1 = 1
        vmovaps xmm0, oword [rdi]
        vmovaps xmm1, oword [rsi]
    %elif %1 = 2
        vmovss xmm1, dword [rdi]
        vmovss xmm2, dword [rsi]
    %elif %1 = 3
        movaps xmm0, oword [rdi]
        movaps xmm1, oword [rsi]
    %elif %1 = 4
        movss xmm1, dword [rdi]
        movss xmm2, dword [rsi]
    %endif

%endmacro

global dot_prod_avx_p
dot_prod_avx_p:
    prologue 1

    ; alternative solution
    ;vmulps xmm0,xmm0, xmm1
    ;vhaddps xmm0,xmm0,xmm3 ;xmm3 is not needed for our solution
    ;vhaddps xmm0,xmm0,xmm3

    vdpps xmm0, xmm0, xmm1, 0xFF

    leave
    ret

global dot_prod_avx_s
dot_prod_avx_s:
    prologue 2

    xor rax, rax
    mov rcx, 4

    cicloAVX:
        vmulss xmm1,xmm1, xmm2
        vaddss xmm0,xmm0,xmm1
        inc rax
        vmovss xmm1, [rdi+rax*4]
        vmovss xmm2, [rsi+rax*4]
    loop cicloAVX

    leave
    ret

global dot_prod_sse_p
dot_prod_sse_p:
    prologue 3

    ; alternative solution
    ;mulps xmm0, xmm1
    ;haddps xmm0,xmm3
    ;haddps xmm0,xmm3
    
    dpps xmm0, xmm1, 0xFF

    leave
    ret

global dot_prod_sse_s
dot_prod_sse_s:
    prologue 4

    xor rax, rax
    mov rcx, 4

    cicloSSE:
        mulss xmm1, xmm2
        addss xmm0,xmm1
        inc rax
        movss xmm1, [rdi+rax*4]
        movss xmm2, [rsi+rax*4]
    loop cicloSSE
    
    leave
    ret
 



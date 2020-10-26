section .text

%macro prologue 1

    push rbp
    mov rbp, rsp
    %if %1 = 1
        vmovdqa xmm1, oword [rdi]
        vmovdqa xmm2, oword [rsi]
    %elif %1 = 2
        vmovd xmm1, [rdi]
        vmovd xmm2, [rsi]
    %elif %1 = 3
        movdqa xmm1, oword [rdi]
        movdqa xmm2, oword [rsi]
    %elif %1 = 4
        movd xmm1, [rdi]
        movd xmm2, [rsi]
    %endif

%endmacro

global dot_prod_avx_p
dot_prod_avx_p:
    prologue 1
    vpmulld xmm0, xmm1, xmm2
    vphaddd xmm0,xmm0,xmm0
    vphaddd xmm0,xmm0,xmm0
    movd eax, xmm0

    leave
    ret

global dot_prod_avx_s
dot_prod_avx_s:
    prologue 2

    leave
    ret

global dot_prod_sse_p
dot_prod_sse_p:
    prologue 3

    leave
    ret

global dot_prod_sse_s
dot_prod_sse_s:
    prologue 4

    leave
    ret



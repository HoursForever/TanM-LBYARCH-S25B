default rel

section .data
    align 16
    ; 2 packed double precision constants [255.0, 255.0] (128 bits / 16 bytes)
    scale dq 255.0, 255.0

section .text
global imgCvtGrayDoubleToInt

imgCvtGrayDoubleToInt:
    ; RCX = height
    ; RDX = width
    ; R8  = input pointer  (const double*)
    ; R9  = output pointer (int*)

    mov     rax, rcx
    imul    rax, rdx            

    test    rax, rax
    jz      .done

    movapd  xmm1, [rel scale]  

    mov     rcx, rax
    shr     rcx, 1              
    jz      .tail_check

.vector_loop:
    movupd    xmm0, [r8]       
    mulpd     xmm0, xmm1        
    cvtpd2dq  xmm0, xmm0        
    movq      [r9], xmm0        

    add       r8, 16            
    add       r9, 8             
    dec       rcx
    jnz       .vector_loop

.tail_check:
    test    rax, 1              
    jz      .done

.tail_handle:
    ; Handle leftover pixel using PACKED operations to strictly avoid scalar SIMD
    sub     rsp, 16             
    mov     rdx, [r8]
    mov     [rsp], rdx          
    mov     qword [rsp + 8], 0  

    movupd    xmm0, [rsp]       
    mulpd     xmm0, xmm1        
    cvtpd2dq  xmm0, xmm0        
    movd      edx, xmm0         
    mov       [r9], edx         
    add       rsp, 16

.done:
    ret
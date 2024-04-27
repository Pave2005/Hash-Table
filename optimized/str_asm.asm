section .text

global strcmp_asm

strcmp_asm: vmovdqu   xmm1, [rdi]
            vpcmpeqd  xmm0, xmm1, [rsi]
            vmovmskps eax, xmm0

            xor eax, 0xFF

            ret


.file   "isrgw.s"
.text
.align 4
.globl _VKSPProtISR
_VKSPProtISR:
        pushl %eax
        popl %eax
        iret

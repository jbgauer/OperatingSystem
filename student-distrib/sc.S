
.globl retrieveBasePointer 
.globl syscall_handler
.globl flush_tlb
.globl iret_context
#define ASM 1

#include "x86_desc.h"

/* retrieveBasePointer
   DESC- grabs the base pointer and returns.
*/
retrieveBasePointer:
    pushl %ebx
    pushl %ecx 
    pushl %edx 
    pushl %esi 
    pushl %edi

    movl %ebp, %eax

    popl %edi 
    popl %esi
    popl %edx 
    popl %ecx
    popl %ebx
    
    ret


/*
 * syscall_handler
 *   DESCRIPTION: handler for system calls
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: 
 */
syscall_handler:

    pushl   %ebp
    pushl   %edi
    pushl   %esi
    pushl   %edx
    pushl   %ecx
    pushl   %ebx

    cmpl $10, %eax       /* 10 sys calls total */
    ja   invalid_syscall

    cmpl $0, %eax        /* no 0 entry in sys call table */
    je   invalid_syscall 

    call *sys_call_table(, %eax, 4)   /* 4 bytes per entry */
    jmp  exit

invalid_syscall:
    movl  $-1, %eax      

exit:
    
    popl  %ebx
    popl  %ecx
    popl  %edx
    
    popl  %esi
    popl  %edi
    popl  %ebp

    iret


sys_call_table:
    .long 0x0          
    .long halt
    .long execute
    .long read
    .long write
    .long open
    .long close
    .long getargs
    .long vidmap
    .long set_handler
    .long sigreturn 

flush_tlb: 
    movl %cr3, %eax
    movl %eax, %cr3
    ret

iret_context:

/* IRET needs 5 elements on stack: User DS, ESP, EFLAG,CS,EIP */

    /* store EIP */
    movl 4(%esp), %eax

    pushl $USER_DS

    /* ESP to User Stack */
    movl $0x08400000, %ebx
    pushl %ebx

    /* set IF flag to 1 */
    pushfl
    popl %ebx
    orl $0x0200, %ebx
    pushl %ebx

    pushl $USER_CS

    /* push eip */
    pushl %eax

    iret




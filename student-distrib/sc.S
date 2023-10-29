
.globl retrieveBasePointer 
.globl syscall_handler
.globl flush_tlb
#define ASM 1

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

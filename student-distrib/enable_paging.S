# enable_paging.S - Enables paging and mixed page sizes, as well as loads the page directory to the processor
.globl enable_paging
.globl load_page_directory
#define ASM     1

#  enable_paging
# DESCRIPTION: sets the control registers (CR4 and CR0) to allow mixed page sizes
#  and to set paging
# INPUTS: none
# OUTPUTS: none
# RETURN VALUE: none
# SIDE EFFECTS: changes bits in the control registers C4 and C0
enable_paging:
    pushl %EBP
    movl %esp, %ebp
    pushl %eax 
    pushl %ebx
    pushl %ecx 
    pushl %edx 
    pushl %esi 
    pushl %edi 

    # set bit 4 of CR4 (PSE) to 1 to allow mixed page sizes
    movl %CR4, %EAX             
    orl $0x00000010, %EAX       
    movl %EAX, %CR4             

    # set paging (PG) and set protection (PE) bits in CR0
    movl %CR0, %EAX             
    orl $0x80000001, %EAX       
    movl %EAX, %CR0             

    popl %edi 
    popl %esi
    popl %edx 
    popl %ecx
    popl %ebx
    popl %eax
    leave
    ret


#  load_page_directory
# DESCRIPTION:  move the page directory pointer into CR3
# INPUTS: pointer to page directory
# OUTPUTS: none
# RETURN VALUE: none
# SIDE EFFECTS: changes CR3 to be the page directory pointer
load_page_directory:                        
    pushl %EBP
    movl %esp, %ebp
    pushl %eax 
    pushl %ebx
    pushl %ecx 
    pushl %edx 
    pushl %esi 
    pushl %edi               

    # put page directory pointer into EAX
    movl 8(%EBP), %EAX                  
    # put the page directory pointer into CR3
    movl %EAX, %CR3                     

    popl %edi 
    popl %esi
    popl %edx 
    popl %ecx
    popl %ebx
    popl %eax
    leave
    ret                             


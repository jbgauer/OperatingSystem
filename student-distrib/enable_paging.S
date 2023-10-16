# enable_paging.S - Enables paging and mixed page sizes, as well as loads the page directory to the processor
.globl enable_paging
.globl load_page_directory
#define ASM     1

enable_paging:
    pushl %EBP
    movl %esp, %ebp
    # set bit 4 of CR4 (PSE) to 1 to allow mixed page sizes
    movl %CR4, %EAX             
    orl $0x00000010, %EAX       
    movl %EAX, %CR4             

    # set paging (PG) and set protection (PE) bits in CR0
    movl %CR0, %EAX             
    orl $0x80000001, %EAX       
    movl %EAX, %CR0             

    leave
    ret


load_page_directory:                        
    pushl %EBP
    movl %esp, %ebp                 
    
    # put page directory pointer into EAX
    movl 8(%EBP), %EAX                  
    # put the page directory pointer into CR3
    movl %EAX, %CR3                     

    leave
    ret                                


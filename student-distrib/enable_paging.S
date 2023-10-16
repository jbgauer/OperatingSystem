# enable_paging.S - Enables paging and mixed page sizes, as well as loads the page directory to the processor

enable_paging:
    pushl %EBP  
    movl %ESP, %EBP  
    pushl %EBX
    pushl %ESI 
    pushl %EDI 

    # set paging (PG) and set protection (PE) bits in CR0
    movl %CR0, %EAX
    orl $0x80000001, %EAX
    movl %EAX, %CR0
    
    # set bit 4 of CR4 (PSE) to 1 to allow mixed page sizes
    movl %CR4, %EAX
    orl $0x00000010, %EAX
    movl %EAX, %CR4
    
    popl %EDI 
    popl %ESI 
    popl %EBX 
    leave
    ret

load_page_directory:
    pushl %EBP  
    movl %ESP, %EBP  
    pushl %EBX
    pushl %ESI 
    pushl %EDI 

    # get page_directory pointer from "paging.c"
    pushl %ECX 
    pushl %EDX 
    call page_init     # in paging.c
    popl %EDX 
    popl %ECX
    
    # put the page directory starting addr into CR3
    movl %EAX, %CR3

    popl %EDI 
    popl %ESI 
    popl %EBX 
    leave
    ret

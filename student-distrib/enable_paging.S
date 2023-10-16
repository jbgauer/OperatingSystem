# enable_paging.S - Enables paging and mixed page sizes, as well as loads the page directory to the processor

#define ASM     1

#define enable_wrap(description) \
    .globl description          ;\
description:                    ;\
    pushfl                      ;\
    pushal                      ;\

    # set paging (PG) and set protection (PE) bits in CR0
    movl %CR0, %EAX             ;\
    orl $0x80000001, %EAX       ;\
    movl %EAX, %CR0             ;\
    
    # set bit 4 of CR4 (PSE) to 1 to allow mixed page sizes
    movl %CR4, %EAX             ;\
    orl $0x00000010, %EAX       ;\
    movl %EAX, %CR4             ;\

    popal                       ;\
    popfl                       ;\
    iret                        ;\


#define load_directory_wrap(description) \
    .globl description                  ;\
description:                            ;\
    pushfl                              ;\
    pushal                              ;\

    # put page directory pointer into EAX
    movl 8(%EBP), %EAX                  ;\
    # put the page directory pointer into CR3
    movl %EAX, %CR3                     ;\

    popal                               ;\
    popfl                               ;\
    iret                                ;\


enable_wrap(enable_paging)
load_directory_wrap(load_page_directory)
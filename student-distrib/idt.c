
#include "idt.h"


void init_idt(){
    int i;
    for (i=0; i<NUM_VEC; i++){
        // used appendix D to set idt up
        idt[i].present = 0;             //set to 0 at start
        if(i==0x80) idt[i].dpl = 3;     //“system call handler” IDT entry at index 0x80
        else        idt[i].dpl = 0;     //priority lvl at 3

        idt[i].size = 1; // 32 bit 
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        if (i<=0x1F || i>=0x30) idt[i].reserved3 = 1; //trap gate, exceptions 0x0-x1F and soft interrupts 0x30-x100 are trap gates 
        else                    idt[i].reserved3 = 0; //interrupt gate

        // 8 bit reserved part of descriptor
        idt[i].reserved4 = 0; //probably dont have to set this cuz its reserved anyway   

        idt[i].seg_selector = KERNEL_CS; 
    }

    // to handle exceptions prob have to SET_IDT_ENTRY for each exception
    // have to make a handler to use this function

    // also handle interrupts and system calls with set idt entry 
    
}

// /* Sets runtime parameters for an IDT entry */
// #define SET_IDT_ENTRY(str, handler)                              \
// do {                                                             \
//     str.offset_31_16 = ((uint32_t)(handler) & 0xFFFF0000) >> 16; \
//     str.offset_15_00 = ((uint32_t)(handler) & 0xFFFF);           \
// } while (0)




// /* An interrupt descriptor entry (goes into the IDT) */
// typedef union idt_desc_t {
//     uint32_t val[2];
//     struct {
//         uint16_t offset_15_00;
//         uint16_t seg_selector;
//         uint8_t  reserved4;
//         uint32_t reserved3 : 1;
//         uint32_t reserved2 : 1;
//         uint32_t reserved1 : 1;
//         uint32_t size      : 1;
//         uint32_t reserved0 : 1;
//         uint32_t dpl       : 2;
//         uint32_t present   : 1;
//         uint16_t offset_31_16;
//     } __attribute__ ((packed));
// } idt_desc_t;

// /* The IDT itself (declared in x86_desc.S */
// extern idt_desc_t idt[NUM_VEC];

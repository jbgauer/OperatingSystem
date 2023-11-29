#include "idt.h"

#include "lib.h"
//#include "lib.c"

/*
 * init_idt
 *   DESCRIPTION: initializes the interrupt descriptor table
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
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
    
    SET_IDT_ENTRY(idt[0x00], divide_by_zero); //sets present bit to 1 as well
    SET_IDT_ENTRY (idt[0x01], debug);
    SET_IDT_ENTRY (idt[0x02], non_maskable_interrupt);
    SET_IDT_ENTRY (idt[0x03], breakpoint);
    SET_IDT_ENTRY (idt[0x04], overflow);
    SET_IDT_ENTRY (idt[0x05], bound_range_exceeded);
    SET_IDT_ENTRY (idt[0x06], invalid_opcode);
    SET_IDT_ENTRY (idt[0x07], device_not_available);
    SET_IDT_ENTRY (idt[0x09], coprocessor_segment_overrun);
    // xF reserved
    SET_IDT_ENTRY (idt[0x10], x87_floatingpoint_exception);
    SET_IDT_ENTRY (idt[0x12], machine_check);
    SET_IDT_ENTRY (idt[0x13], SIMD_floatingpoint_exception);

    SET_IDT_ENTRY (idt[0x08], double_fault);
    SET_IDT_ENTRY (idt[0x0a], invalid_tss);
    SET_IDT_ENTRY (idt[0x0b], segment_not_present);
    SET_IDT_ENTRY (idt[0x0c], stacksegment_fault);
    SET_IDT_ENTRY (idt[0x0d], general_protection_fault);
    SET_IDT_ENTRY (idt[0x0e], page_fault);
    SET_IDT_ENTRY (idt[0x11], alignnment_check);

    SET_IDT_ENTRY(idt[0x20], pit_handler); 
    
    SET_IDT_ENTRY(idt[0x21], keyboard_interrupt_handler); 

    SET_IDT_ENTRY(idt[0x28], rtc_interrupt_handler);  // rtc

    SET_IDT_ENTRY(idt[0x80], syscall_handler); // system call 
    
}

// /* Sets runtime parameters for an IDT entry */
// #define SET_IDT_ENTRY(str, handler)                              
// do { 
//     str.present = 1;                                                            
//     str.offset_31_16 = ((uint32_t)(handler) & 0xFFFF0000) >> 16; 
//     str.offset_15_00 = ((uint32_t)(handler) & 0xFFFF);           
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


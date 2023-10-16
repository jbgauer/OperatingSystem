#include "handlers.h"


char* exceptions[20] = {"Division by Zero", "debug", "non_maskable_interrupt", "breakpoint", "overflow", 
"bound_range_exceeded", "invalid_opcode", "device_not_available", "double_fault", "coprocessor_segment_overrun", 
"invalid_tss", "segment_not_present", "stacksegment_fault", "general_protection_fault", "page_fault", 
"x87_floatingpoint_exception", "alignnment_check", "machine_check", "SIMD_floatingpoint_exception"};

/*
 * exception_handler
 *   DESCRIPTION: exception handler
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints the exception and freezes
 */
void exception_handler(uint32_t vectorNo){
    cli();
    
    volatile int dontmodify = 0;
    printf("Exception: %s", exceptions[vectorNo]);
    
    while(1){   //freeze
        if(dontmodify) break;
    }
}


// keyboard and rtc handlers in devices .c



/*
 * syscall_handler
 *   DESCRIPTION: handler for system calls
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints the system call and freezes
 */
void syscall_handler(){
    cli();

    volatile int dontmodify = 0;
    printf("System Call was Called");
    
    while(1){   //freeze
        if(dontmodify) break;
    }

}











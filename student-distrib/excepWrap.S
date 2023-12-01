
#define ASM     1

/* handler wrappers go into SET_IDT_ENTRY for exceptions, interrupts, and system calls

 need it so we know what exception was triggered

 no error code written so dont need to flush it */
#define EXCEPTION_WRAPPER(description, vectorNo) \
    .globl description                          ;\
    description:                                ;\
        pushfl                                  ;\
        pushal                                  ;\
        pushl $vectorNo                         ;\
        call exception_handler                  ;\
        addl $4, %esp                           ;\
        popal                                   ;\
        popfl                                   ;\
        iret

/* flush error code */
#define EXCEPTION_WRAPPER_ERR_CODE(description, vectorNo)    \
    .globl description                                      ;\
    description:                                            ;\
        pushfl                                              ;\
        pushal                                              ;\
        pushl $vectorNo                                     ;\
        call exception_handler                              ;\
        addl $4, %esp                                       ;\
        popal                                               ;\
        popfl                                               ;\
        addl $4, %esp                                       ;\
        iret

#define INTERRUPTWRAPPER(description, handler) \
    .globl description                   ;\
    description:                         ;\
        pushfl                           ;\
        pushal                           ;\
        call handler                     ;\
        popal                            ;\
        popfl                            ;\
        iret

EXCEPTION_WRAPPER (divide_by_zero, 0x00);
EXCEPTION_WRAPPER (debug, 0x01);
EXCEPTION_WRAPPER (non_maskable_interrupt, 0x02);
EXCEPTION_WRAPPER (breakpoint, 0x03);
EXCEPTION_WRAPPER (overflow, 0x04);
EXCEPTION_WRAPPER (bound_range_exceeded, 0x05);
EXCEPTION_WRAPPER (invalid_opcode, 0x06);
EXCEPTION_WRAPPER (device_not_available, 0x07);
EXCEPTION_WRAPPER (coprocessor_segment_overrun, 0x09);
/* xF reserved */
EXCEPTION_WRAPPER (x87_floatingpoint_exception, 0x10);
EXCEPTION_WRAPPER (machine_check, 0x12);
EXCEPTION_WRAPPER (SIMD_floatingpoint_exception, 0x13);

EXCEPTION_WRAPPER_ERR_CODE (double_fault, 0x08);
EXCEPTION_WRAPPER_ERR_CODE (invalid_tss, 0x0a);
EXCEPTION_WRAPPER_ERR_CODE (segment_not_present, 0x0b);
EXCEPTION_WRAPPER_ERR_CODE (stacksegment_fault, 0x0c);
EXCEPTION_WRAPPER_ERR_CODE (general_protection_fault, 0x0d);
EXCEPTION_WRAPPER_ERR_CODE (page_fault, 0x0e);
EXCEPTION_WRAPPER_ERR_CODE (alignnment_check, 0x11);



INTERRUPTWRAPPER (rtc_interrupt_handler, rtc_handler);

INTERRUPTWRAPPER (keyboard_interrupt_handler, keyboard_handler);

INTERRUPTWRAPPER(pit_interrupt_handler, pit_handler)

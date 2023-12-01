

#ifndef _HANDLERWRAP_H
#define _HANDLERWRAP_H

#include "exceptions.h"
#include "keyboard.h"
#include "pit.h"


// handler wrappers for exceptions
void divide_by_zero();
void debug();
void non_maskable_interrupt();
void breakpoint();
void overflow();
void bound_range_exceeded();
void invalid_opcode();
void device_not_available();
void coprocessor_segment_overrun();
void x87_floatingpoint_exception();
void machine_check();
void SIMD_floatingpoint_exception();
void double_fault();
void invalid_tss();
void segment_not_present();
void stacksegment_fault();
void general_protection_fault();
void page_fault();
void alignnment_check();

// handler wrapper for rtc interrupts
void rtc_interrupt_handler();

// handler wrapper for keyboard interrupts
void keyboard_interrupt_handler();

// handler wrapper for pit interrupts
void pit_interrupt_handler();


#endif

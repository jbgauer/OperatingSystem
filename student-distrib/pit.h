#ifndef _PIT_H
#define _PIT_H

#include "lib.h"
#include "i8259.h"

#define PIT_IRQ 0
#define PIT_DATA 0x40
#define PIT_CMD 0x43
#define PIT_DEF_FREQ 1193180
#define PIT_FREQ 100

void pit_init();
void pit_handler();



#endif

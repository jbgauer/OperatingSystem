#ifndef _IDT_H_
#define _IDT_H_


#include "x86_desc.h"
#include "excepWrap.h"
#include "exceptions.h"
#include "keyboard.h"
#include "rtc.h"

#include "syscall.h"
#include "pit.h"

void init_idt();


#endif

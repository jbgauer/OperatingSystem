#ifndef _IDT_H_
#define _IDT_H_


#include "x86_desc.h"
#include "handlerWrap.h"
#include "handlers.h"
#include "keyboard.h"
#include "rtc.h"

#include "syscall_link.h"

void init_idt();


#endif

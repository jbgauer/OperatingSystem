
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "lib.h"
#include "filesys_driver.h"
#include "paging.h"
#include "x86_desc.h"
#include "pcb.h"
#include "rtc.h"

#define COMMAND_MAX 128
#define EXEC_VAL 0x464C457F
#define PROG_MAX 6
#define KENTRY_SHIFT 10
#define PAGES_DEFAULT_USE 2
#define FILE_SIZE (4096*1023)
#define KERNEL_DS 0x0018
#define PRGRM_IMG_START 0x08048000
#define FILE_MAX 8
#define USER_SPACE 32

// current process id
uint32_t curr_pid;

extern uint32_t retrieveBasePointer();

extern void syscall_handler();

extern void flush_tlb();

extern void iret_context(uint32_t eip);


int32_t halt (uint8_t status);

int32_t execute(const uint8_t *command);

int32_t read (int32_t fd, void* buf, int32_t nbytes);

int32_t write (int32_t fd, const void* buf, int32_t nbytes);

int32_t open (const uint8_t* filename);

int32_t close (int32_t fd);

int32_t getargs (uint8_t* buf, int32_t nbytes);

int32_t vidmap (uint8_t** screen_start);

int32_t set_handler(int32_t signum, void* handler_address);

int32_t sigreturn (void);

#endif

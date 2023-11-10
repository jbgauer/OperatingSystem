/*
 * paging.h - Definitions for paging
*/

#ifndef _PAGING_H
#define _PAGING_H

#include "types.h"
#include "enable_paging.h"
#include "lib.h"

// [PD index, PT index, pageoffset]
// ^ this is what the virtual address will look like
// pageddir[PDindex] = pagetable
// pagetable[PTindex] = pageoffset

//directory is array of pointers to page tables
//table is array of pointers to pages
//offset is into the page itself (NOT the page table)
uint32_t pagedir[1024] __attribute__((aligned(4096)));
uint32_t first_pagetable[1024] __attribute__((aligned(4096)));
uint32_t page_table_vmem[1024] __attribute__((aligned(4096)));
//these structs for dir and table entries are just to help organize
//the actual tables are just 32-bit numbers (uint32_t)
typedef struct {
    uint32_t p_addr; //31-12 pagetable address
    
    // 11-8 is empty space (Available)
    uint32_t ps;    //7 Page Size
    //bit 6 is empty space
    uint32_t a;     //5 Accessed
    uint32_t pcd;   //4 Cache-Disable
    uint32_t pwt;   //3 Write-Thru
    uint32_t us;    //2 User/Supervisor
    uint32_t rw;    //1 Read/Write
    uint32_t p;     //0 Present
} pdir_entry_t;

typedef struct {
    uint32_t p_addr; //31-22: 31-22 of address
    //bits 21-13 are reserved
    uint32_t pat; //12: Page attribute table
    //bits 11-9 is empty space (Available)
    uint32_t g;     //8: Global
    uint32_t ps;    //7: page size
    uint32_t d;     //6: dirty
    uint32_t a;     //5: Accessed
    uint32_t pcd;   //4 Cache-Disable
    uint32_t pwt;   //3 Write-Thru
    uint32_t us;    //2 User/Supervisor
    uint32_t rw;    //1 Read/Write
    uint32_t p;     //0 Present
} pdir_entry_MB_t;

typedef struct {
    uint32_t m_addr; //31-12 physical memory address
    // 11-9 is just empty space (Available)
    //variables to hold each bit
    uint32_t g;     //8 Global
    uint32_t pat;   //7 Page Attribute
    uint32_t d;     //6 Dirty
    uint32_t a;     //5 Accessed
    uint32_t pcd;   //4 Cache-Disable
    uint32_t pwt;   //3 Write-Thru
    uint32_t us;    //2 User/Supervisor
    uint32_t rw;    //1 Read/Write
    uint32_t p;     //0 Present
} ptable_entry_t;


void page_init();

//these functions take all the separate variables of the struct
// and combine them into one 32 bit (4 byte) value
uint32_t combine_dir_entry(pdir_entry_t pde);
uint32_t combine_table_entry(ptable_entry_t pte);
uint32_t combine_dir_entry_MB(pdir_entry_MB_t pde);

#endif

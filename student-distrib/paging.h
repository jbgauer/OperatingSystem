/*
 * paging.h - Definitions for paging
*/

#include "types.h"

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


uint32_t* page_init();
/*
 * paging.c - Paging functionality and initialization
*/

#include "paging.h"

page_t pagedir[1024];
page_t pagetable[1024];


/*
 * page_init
 *   DESCRIPTION: initialize a single page
 *   INPUTS: 
 *   OUTPUTS: 
 *   RETURN VALUE: 
 *   SIDE EFFECTS: 
 */
 page_t page_init(){
    page_t curpage;
    curpage.s_addr = 0;

    return curpage;
 }

//set CR3 to the starting address of the page directory (index 0)
    //look in "x86_desc.S"
//directory is array of pointers to page tables
//table is array of pointers to pages
//offset is into the page itself (NOT the page table)

page_t* paging_init(){
    //return the pointer to the start of page directory
    // [PD index, PT index, pageoffset]
    // ^ this is what the virtual address will look like
    // pageddir[PDindex] = pagetable
    // pagetable[PTindex] = pageoffset

    // pagedir[1] = pagetable;
    // pagetable[?] = pageoffset;
    
    return &pagedir[0];
}

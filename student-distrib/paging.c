/*
 * paging.c - Paging functionality and initialization
*/

#include "paging.h"

//directory is array of pointers to page tables
//table is array of pointers to pages
//offset is into the page itself (NOT the page table)
uint32_t pagedir[1024] __attribute__((aligned(4096)));

//these functions take all the separate variables of the struct
// and combine them into one 32 bit (4 byte) value
uint32_t combine_dir_entry(pdir_entry_t pde);
uint32_t combine_table_entry(ptable_entry_t pte);

//take a combo_entry and return a struct that has all the separate variables
// pdir_entry_t uncombine_dir_entry(uint32_t);
// ptable_entry_t uncombine_table_entry(uint32_t);

/*
 * page_init
 *   DESCRIPTION: initialize the page directory (and page tables inside it)
        set all the entries of the directory and tables
 *   INPUTS: none
 *   OUTPUTS: 32 bit value that is address of page directory
 *   RETURN VALUE: pointer to start of page directory
 *   SIDE EFFECTS: creates page table
 */
uint32_t* page_init(){

    //return the pointer to the start of page directory
    // [PD index, PT index, pageoffset]
    // ^ this is what the virtual address will look like
    // pageddir[PDindex] = pagetable
    // pagetable[PTindex] = pageoffset

    uint32_t kernel_pagetable[1024] __attribute__((aligned(4096)));

    //initialize the page directory
    int i;
    for(i=0; i < 1024; i++){
        //set supervisor level (only kernel can access)
        //set r/w to 1 to allow writing
        //set table is not present
        pagedir[i] = 0x00000002;
    }

    //want to start mapping  KERNEL pages at 4MB
    for(i=0; i < 1024; i++){
        //or with 011 to set r/w and set page IS present
        kernel_pagetable[i] = ((i+1024) * 0x1000) | 3;
    }

    //put this table into the directory
    pagedir[1] = ((unsigned int)kernel_pagetable) | 3;
    //do this cast ^ to get the addr of the page table

    return &pagedir[1];
 }

/*
 * combine_dir_entry
 *   DESCRIPTION: gets the combined 32-bit entry of a pdir entry struct
 *   INPUTS: a pdir entry struct
 *   OUTPUTS: combined entry
 *   RETURN VALUE: 32-bit int (unsigned)
 *   SIDE EFFECTS: none
 */
uint32_t combine_dir_entry(pdir_entry_t pde){
    uint32_t combo;

    combo = pde.p_addr << 12;

    combo += pde.ps << 7;
    combo += pde.a << 5;
    combo += pde.pcd << 4;
    combo += pde.pwt << 3;
    combo += pde.us << 2;
    combo += pde.rw << 1;
    combo += pde.p;

    return combo;
}

/*
 * combine_table_entry
 *   DESCRIPTION: gtes the combined 32-bit entry of a ptable entry struct
 *   INPUTS: a ptable entry struct
 *   OUTPUTS: combined entry
 *   RETURN VALUE: 32-bit int (unsigned)
 *   SIDE EFFECTS: none
 */
uint32_t combine_table_entry(ptable_entry_t pte){
    uint32_t combo;

    combo = pte.m_addr << 12;

    combo += pte.g << 8;
    combo += pte.pat << 7;
    combo += pte.d << 6;
    combo += pte.a << 5;
    combo += pte.pcd << 4;
    combo += pte.pwt << 3;
    combo += pte.us << 2;
    combo += pte.rw << 1;
    combo += pte.p;

    return combo;
}

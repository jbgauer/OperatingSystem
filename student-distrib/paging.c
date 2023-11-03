/*
 * paging.c - Paging functionality and initialization
*/

#include "paging.h"


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
void page_init(){
    int i;
    uint32_t vidf, kernf;
    //video memory ptable entry
    ptable_entry_t vidmem;
    //kernel pdir entry
    pdir_entry_MB_t kerntry;

    vidmem.m_addr = 0xB8;
    vidmem.g = 0;
    vidmem.pat = 0;
    vidmem.d = 0;
    vidmem.a = 0;
    vidmem.pcd = 0;
    vidmem.pwt = 0;
    vidmem.us = 0;
    vidmem.rw = 1;
    vidmem.p = 1;

    kerntry.p_addr = 0x400;
    kerntry.pat = 0;
    kerntry.g = 0;
    kerntry.ps = 1;
    kerntry.d = 0;
    kerntry.a = 0;
    kerntry.pcd = 0;
    kerntry.pwt = 0;
    kerntry.us = 0;
    kerntry.rw = 1;
    kerntry.p = 1;

    vidf = combine_table_entry(vidmem);
    kernf = combine_dir_entry_MB(kerntry);
    
    //initialize the page directory
    for(i=0; i < 1024; i++){
        //set supervisor level (only kernel can access)
        //set r/w to 1 to allow writing
        //set table is not present
        pagedir[i] = 0x00000002;
    }

    //initialize the first page table
    for(i=0; i < 1024; i++){
        //set supervisor level (only kernel can access)
        //set r/w to 1 to allow writing
        //set table is not present
        first_pagetable[i] = 0x00000002;
    }

    //page table entry for the video memory, 4kB, with w/r and present bits on
    first_pagetable[0xB8] = vidf; //0x000B8003;

    //put video memory table into the directory
    pagedir[0] = ((unsigned int)first_pagetable) | 3;
    //do this cast ^ to get the addr of the page table

    //page directory entry for the Kernel map, 4MB, with w/r and present bits on
    pagedir[1] = kernf; //0x00400083;

    load_page_directory(&pagedir[0]);
    enable_paging();
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
 * combine_dir_entry_MB
 *   DESCRIPTION: gets the combined 32-bit entry of a 4MB pdir entry struct
 *   INPUTS: a 4MB pdir entry struct
 *   OUTPUTS: combined entry
 *   RETURN VALUE: 32-bit int (unsigned)
 *   SIDE EFFECTS: none
 */
uint32_t combine_dir_entry_MB(pdir_entry_MB_t pde){
    uint32_t combo;

    combo = pde.p_addr << 12;
    combo += pde.pat << 12;
    combo += pde.g << 8;
    combo += pde.ps << 7;
    combo += pde.d << 6;
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

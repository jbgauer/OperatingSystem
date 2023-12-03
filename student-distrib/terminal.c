#include "terminal.h"
#include "keyboard.h"

/*
 * term_init
 *   DESCRIPTION: initialize term structure
 *   INPUTS: uint32_t - number for index of terminal to initialize
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void
term_init(uint32_t num, int screen_x, int screen_y) {
    curr_terminal = num;
    int i;
    uint32_t newEntry;
    ptable_entry_t vidmem1;
    //uint32_t test;
    terminal[num].enter_flag = 0;
    terminal[num].buf_i = 0;
    terminal[num].scr_x = screen_x;
    terminal[num].scr_y = screen_y;
    terminal[num].programs_running = 0;
    for(i = 0; i < KEYBOARD_BUF_LEN; i++) {
        terminal[num].key_buf[i] = 0;
    }

    terminal[num].count = 0;
    terminal[num].num_ticks = 1;

    //set pages
    vidmem1.m_addr = (VIDEO_ADDR + (num+1)*PAGE_SIZE)>>PAGE_SHIFT;
    vidmem1.g = 0;
    vidmem1.pat = 0;
    vidmem1.d = 0;
    vidmem1.a = 0;
    vidmem1.pcd = 0;
    vidmem1.pwt = 0;
    vidmem1.us = 1;
    vidmem1.rw = 1;
    vidmem1.p = 1;
    newEntry = combine_table_entry(vidmem1);

    first_pagetable[(VIDEO_ADDR + (num+1)*PAGE_SIZE)>>PAGE_SHIFT] = newEntry;
    //flush tlb
    flush_tlb();
}

/*
 * term_open
 *   DESCRIPTION: initialize term variables
 *   INPUTS: filename
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failed
 *   SIDE EFFECTS: none
 */
int32_t
term_open(const uint8_t* filename) {
    /*success*/
    return 0;
}

/*
 * term_close
 *   DESCRIPTION: clears any terminal specific variables
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failed
 *   SIDE EFFECTS: none
 */
int32_t
term_close(int32_t fd) {
    /*success*/
    return -1;
}

/*
 * term_read
 *   DESCRIPTION: Reads keyboard buff until user hits 'enter' key, copy everything into user buff
 *  and return it to user space.
 *   INPUTS: fd - file descriptor, buf - user buffer, nbytes - # of bytes to copy
 *   OUTPUTS: none
 *   RETURN VALUE: # of bytes written, -1 if failed
 *   SIDE EFFECTS: copies keyboard buff into user buffer and sends it to user space
 */
int32_t 
term_read(int32_t fd, void* buf, int32_t nbytes) {
    int32_t i, j;    
    uint8_t en_flag = 0;
    char* buf1 = (char*)buf;
    term_t* curr_term = &terminal[curr_thread];
    /*check if buf is valid*/
    if(buf1 == NULL || (nbytes < 1)) {
        /*failed*/
        return -1;
    }
    //reset enter_flag
    curr_term->enter_flag = 0;
    //wait until enter is pressed
    while(!curr_term->enter_flag) {
        ;
    }
    //user has hit enter
    /*Copy keyboard buff to user buff*/
    for(i = 0; i < nbytes; i++) {
        buf1[i] = curr_term->key_buf[i];
        if(curr_term->key_buf[i] == '\n') {
            en_flag = 1;
            break;
        }
    }
    //check if last char is \n if not, set it.
    if(en_flag == 0) {
        buf1[nbytes] = '\n';
    }
    //clear keyboard buf
    for(j = 0; j < KEYBOARD_BUF_LEN; j++) {
        curr_term->key_buf[j] = 0; 
    }
    curr_term->buf_i = 0;
    /*success*/
    return i;
}


/*
 * term_write
 *   DESCRIPTION: copy the user buffer to the screen
 *   INPUTS:  const void* - buf    - buffer to be written to 
 *            int32_t     - nbytes - number of bytes to write 
 *   OUTPUTS: user buffer 
 *   RETURN VALUE: number of bytes written, -1 if failed
 *   SIDE EFFECTS: writes to the screen
 */
int32_t
term_write(int32_t fd, const void* buf, int32_t nbytes) {
    /*initialize variables*/
    char* buf1 = (char*)buf;
    int32_t byteCount = 0;
    int32_t i = 0;
    /*check if buf is valid*/
    if(buf1 == NULL || (nbytes < 1)) {
        /*failed*/
        return -1;
    }
    /*Print to screen if not NULL*/
    for(i = 0; i < nbytes; i++) {
        if(buf1[i] != '\0') {
          putc(buf1[i]);
          byteCount++;
        }
    }
    /*success*/
    return byteCount;
}

/*
 * bad_write
 *   DESCRIPTION: returns negative one for when write is not allowed
 *   INPUTS: none
 *   OUTPUTS: none 
 *   RETURN VALUE: -1
 *   SIDE EFFECTS: Just returns -1
 */
int32_t bad_write (int32_t fd, const void* buf, int32_t nbytes) {
    return -1;
}

/*
 * bad_read
 *   DESCRIPTION: returns negative one for when read is not allowed
 *   INPUTS: none
 *   OUTPUTS: none 
 *   RETURN VALUE: -1
 *   SIDE EFFECTS: Just returns -1
 */
int32_t bad_read (int32_t fd, void* buf, int32_t nbytes) {
    return -1;
}

/*
 * change_terminal
 *   DESCRIPTION: changes terminal from current to different terminal, and prints it to screen.
 *   INPUTS: term_id of the terminal you wish to change to
 *   OUTPUTS: none 
 *   RETURN VALUE: none
 *   SIDE EFFECTS: saves current vidmem, loads in new vidmem, and changes all respective
 *   variables.
 */
void change_terminal(int32_t term_id) {
    cli();
    //check if term_id is valid
    if(term_id > 2 || term_id < 0 || term_id == curr_terminal) {
        return;
    }
    
    //old vidmem location
    uint32_t old_vidmem = VIDEO_ADDR + (curr_terminal+1)*PAGE_SIZE;
    //new vidmem location
    uint32_t new_vidmem = VIDEO_ADDR + (term_id+1)*PAGE_SIZE;

    //Changes vidmap paging
    uint32_t vidf;
    ptable_entry_t vidmem;

    //Changes physical address of the vidmap page of the outgoing terminal
    if(page_table_vmem[curr_terminal] && 1 == 1) {
        vidmem.m_addr = 0xB8+curr_terminal+1;
        vidmem.g = 0;
        vidmem.pat = 0;
        vidmem.d = 0;
        vidmem.a = 0;
        vidmem.pcd = 0;
        vidmem.pwt = 0;
        vidmem.us = 1;
        vidmem.rw = 1;
        vidmem.p = 1;
        vidf = combine_table_entry(vidmem);
        page_table_vmem[curr_terminal] = vidf;
        flush_tlb();
    }

    //Changes physical address of the vidmap page of the incoming terminal
    if(page_table_vmem[term_id] && 1 == 1) {
        vidmem.m_addr = 0xB8;
        vidmem.g = 0;
        vidmem.pat = 0;
        vidmem.d = 0;
        vidmem.a = 0;
        vidmem.pcd = 0;
        vidmem.pwt = 0;
        vidmem.us = 1;
        vidmem.rw = 1;
        vidmem.p = 1;
        vidf = combine_table_entry(vidmem);
        page_table_vmem[term_id] = vidf;
        flush_tlb();
    }

    //save vidmem
    memcpy((char*)old_vidmem,(char*)VIDEO_ADDR, PAGE_SIZE);
    //load in new vidmem
    memcpy((char*)VIDEO_ADDR, (char*)new_vidmem, PAGE_SIZE);

    //change current terminal
    curr_terminal = term_id;
    update_cursor();
    sti();
}




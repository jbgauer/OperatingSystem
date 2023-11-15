#include "terminal.h"
#include "keyboard.h"


/*
 * terminals_init
 *   DESCRIPTION: initialize terminals to not in use
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void
terminals_init() {
    int i;
    for(i = 0; i < MAX_TERMINALS; i++) {
        terminal[i].in_use = 0;
    }
    curr_terminal = 0;
}

/*
 * term_init
 *   DESCRIPTION: initialize term structure
 *   INPUTS: uint32_t - number for index of terminal to initialize
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void
term_init(uint32_t num) {
    int i;
    terminal[num].in_use = 1;
    terminal[num].enter_flag = 0;
    terminal[num].buf_i = 0;
    terminal[num].scr_x = 0;
    terminal[num].scr_y = 0;
    for(i = 0; i < KEYBOARD_BUF_LEN; i++) {
        terminal[num].key_buf[i] = 0;
    }
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
    term_t* curr_term = &terminal[curr_terminal];
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

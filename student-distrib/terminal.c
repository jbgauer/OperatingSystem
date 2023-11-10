#include "terminal.h"
#include "keyboard.h"
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
    /*check if buf is valid*/
    if(buf1 == NULL || (nbytes < 1)) {
        /*failed*/
        return -1;
    }
    //wait until enter is pressed
    while(!enter_flag) {
        ;
    }
    //user has hit enter
    /*Copy keyboard buff to user buff*/
    for(i = 0; i < nbytes; i++) {
        buf1[i] = keyboard_buf[i];
        if(keyboard_buf[i] == '\n') {
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
        keyboard_buf[j] = 0; 
    }
    keyboard_buf_index = 0;
    /*success*/
    //reset enter_flag
    enter_flag = 0;
    return i;
}


/*
 * term_write
 *   DESCRIPTION: copy the user buffer to the screen
 *   INPUTS: none
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

int32_t bad_write (int32_t fd, const void* buf, int32_t nbytes) {
    return -1;
}

int32_t bad_read (int32_t fd, void* buf, int32_t nbytes) {
    return -1;
}

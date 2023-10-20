//#include "terminal.h"

//int32_t keyboard_buf_index; 

/*
 * term_open
 *   DESCRIPTION: initialize term variables
 *   INPUTS: filename
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failed
 *   SIDE EFFECTS: intializes global counter
 */
//int32_t
//term_open(const uint8_t* filename) {
  //  keyboard_buf_index = 0;         //resets buff index
    /*success*/
   // return 0;
//}


/*
 * term_close
 *   DESCRIPTION: clears any terminal specific variables
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if failed
 *   SIDE EFFECTS: clears keyboard_buf and buf_index
 */
//int32_t
//term_close(int32_t fd) {
  //  int32_t i;
  //  keyboard_buf_index = 0;     //reset buff index
    /*Clear keyboard buff*/
  //  for(i = 0; i < BUF_LENGTH; i++) {
  //      keyboard_buf[i] = 0;
 //  }
    /*success*/
 //   return 0;
//}

/*
 * term_read
 *   DESCRIPTION: Reads keyboard buff until user hits 'enter' key, copy everything into user buff
 *  and return it to user space.
 *   INPUTS: fd - file descriptor, buf - user buffer, nbytes - # of bytes to copy
 *   OUTPUTS: none
 *   RETURN VALUE: # of bytes written, -1 if failed
 *   SIDE EFFECTS: copies keyboard buff into user buffer and sends it to user space
 */
//int32_t 
//term_read(int32_t fd, void* buf, int32_t nbytes) {
   // int32_t byteCount = 0;
   // volatile int8_t not_enter = 1;
    /*check if buf is valid*/
  //  if(buf == NULL) {
        /*failed*/
   //     return -1;
  //  }

  //  while(not_enter) {
        /*has enter been pressed*/
   //     if(keyboard_buf[keyboard_buf_index] = /*'enter'*/) {
   //         not_enter = 0;
    //    }
  //  }
    //user has hit enter
    /*Copy keyboard buff to user buff*/
  //  for(i = 0; i < BUF_LENGTH; i++) {
  //      buf[i] = keyboard_buf[i];
 //   }
    //Return it to user space


    /*success*/
 //   return byteCount;
//}

    
/*
 * term_write
 *   DESCRIPTION: copy the user buffer to the screen
 *   INPUTS: none
 *   OUTPUTS: user buffer 
 *   RETURN VALUE: number of bytes written, -1 if failed
 *   SIDE EFFECTS: none
 */
//int32_t
//term_write(int32_t fd, const void* buf, int32_t nbytes) {
    /*initialize variables*/
  //  int32_t byteCount = 0;
  //  int32_t i;
    /*check if buf is valid*/
  //  if(buf == NULL) {
        /*failed*/
  //      return -1;
 //   }
  //  for(i = 0; i < nbytes; i++) {

  //  }

    /*Print to screen*/
  //  printf("%s", );

    /*success*/
  //  return byteCount;
//}


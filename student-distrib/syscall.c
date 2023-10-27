// #include "syscall.h"
// #include "syscall.S"
// #include "filesys_driver.h"

// /*
//  * halt
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: none
//  *   RETURN VALUE: 
//  *   SIDE EFFECTS: none
//  */
// int32_t halt (uint8_t status) {

// }


// /*
//  * system_execute
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: none
//  *   RETURN VALUE: 
//  *   SIDE EFFECTS: none
//  */
// int32_t
// execute(const uint8_t *command) {
        
//     uint8_t *cmdHold;
//     uint8_t *cmdArgs;
//     uint8_t *file;
//     uint32_t i;
//     uint8_t *buf;
//     dentry_t *dentry;
    
//     //uint32_t basePointer;

// //parent base pointer
//     //basePointer = retrieveBasePointer();

//     if(command == NULL) {
//         return -1;
//     }
//     /*Parse Args*/
//     cmdHold = command;
//     //skip spaces
//     for(int i = 0; i < COMMAND_MAX; i++) {
//         if(cmdHold == ' ') {
//             cmdHold++;
//         } else { 
//             break;
//         }
//     } 
//     file = cmdHold;
//     //go to end of filename
//     while(cmdHold != ' ' || cmdHold != '\n' || cmdHold != '\0') {
//         cmdHold++;
//     }
//     //set end of filename
//     *cmdHold = '\0';
//     cmdHold++;
//     cmdArgs = cmdHold;
//     //go to end of args
//     while(cmdHold != ' ' || cmdHold != '\n' || cmdHold != '\0') {
//         cmdHold++;
//     }
//     //set end
//     cmdHold = '\0';

//     /*Executable Check*/
//     //set dentry
//     read_dentry_by_name(file, dentry);
//     if(read_data(dentry->inode_num,0,buf,4) == -1) 
//         return -1;
//     if(buf != EXEC_VAL) 
//         return -1;
//     if(dentry->filetype != 2)
//         return -1;

//     /*set up program paging*/
//     //always at 32
//     //user page (4mb)
//     //once have the page read_data


//     /*user-level program loader*/
//     //sys all in eax, first arg in EBX, ECX, EDX

//     /*create pcb*/

//     /*context switch (IN x86)*/
//         //create its own context switch stack
//         //IRET

// }



// /*
//  * read
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: none
//  *   RETURN VALUE: 
//  *   SIDE EFFECTS: none
//  */
// int32_t read (int32_t fd, void* buf, int32_t nbytes); {

// }

// /*
//  * write
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: none
//  *   RETURN VALUE: 
//  *   SIDE EFFECTS: none
//  */
// int32_t write (int32_t fd, const void* buf, int32_t nbytes) {

// }

// /*
//  * open
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: none
//  *   RETURN VALUE: 
//  *   SIDE EFFECTS: none
//  */
// int32_t open (const uint8_t* filename) {

// }

// /*
//  * close
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: none
//  *   RETURN VALUE: 
//  *   SIDE EFFECTS: none
//  */
// int32_t close (int32_t fd) {

// }

// /*
//  * getargs
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: none
//  *   RETURN VALUE: 
//  *   SIDE EFFECTS: none
//  */
// int32_t getargs (uint8_t* buf, int32_t nbytes) {

// }

// /*
//  * vidmap
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: none
//  *   RETURN VALUE: 
//  *   SIDE EFFECTS: none
//  */
// int32_t vidmap (uint8_t** screen_start) {

// }

// /*
//  * set_handler
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: none
//  *   RETURN VALUE: 
//  *   SIDE EFFECTS: none
//  */
// int32_t set_handler(int32_t signum, void* handler_address) {

// }

// /*
//  * sigreturn
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: none
//  *   RETURN VALUE: 
//  *   SIDE EFFECTS: none
//  */
// int32_t sigreturn (void) {

// }




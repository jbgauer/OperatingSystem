// #include "execute.h"
// #include "lib.h"




// /*
//  * system_execute
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: none
//  *   RETURN VALUE: 
//  *   SIDE EFFECTS: none
//  */
// int32_t
// system_execute(const uint8_t *command) {
        
//     uint8_t *cmdHold;
//     uint8_t *cmdArgs;
//     uint8_t *file;
//     uint32_t i;

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
//     //check for args
//     for(int j = 0; j = COMMAND_MAX-i; j++) {
//         if(cmdHold == ' ' || cmdHold == '\n' || cmdHold == '\0') {
//             cmdHold++;
//             break;
//         }
//         cmdHold++;
//     }
//     cmdArgs = cmdHold;
    
//     /*Executable Check*/
//     //file open check if return -1
//     //read data check if return -1
//     //file type
//     //if 0x464C457F


//     /*set up program paging*/



//     /*user-level program loader*/
//     //sys all in eax, first arg in EBX, ECX, EDX

//     /*create pcb*/

//     /*context switch (IN x86)*/
//         //create its own context switch stack
//         //IRET

// }





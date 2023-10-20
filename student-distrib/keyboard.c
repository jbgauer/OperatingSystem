

#include "keyboard.h"
#include "i8259.h"
#include "lib.h"

// char keyboard_buf[128];
// uint8_t buf_index;

// uint8_t left_shift_flag;
// uint8_t right_shift_flag;
// uint8_t caps_flag;
// uint8_t ctrl_flag;

char scancodeTranslator[NUM_SCANCODES] =       // only putting alphabet and numbers for now, mapping all other keys to show nothing (0x0)
// 83 is all entries included lowercase and all numbers

{0x0, 0x0, '1', '2', '3', '4', '5', '6', '7', '8',  //27 is escape character
 '9', '0', '-', '=', 0x0, 0x0, 'q', 'w', 'e', 'r',  //8 is backspace char, tab
 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0x0, 0x0,  //13 is enter, ctrl
 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 
 '\'','`', 0x0, '\\','z', 'x', 'c', 'v', 'b', 'n',  // Left shift
 'm', ',', '.', '/', 0x0, '*', 0x0, ' ', 0x0, 0x0,  // Right shift, left alt, caps lock, F1
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  // F2, F3, F4, F5, F6, F7, F8, F9, F10, Numlock
 0x0, '7', '8', '9', 0x0, '4', '5', '6', 0x0, '1',  // Scroll lock, keypad -, keypad +
 '2', '3', '0' 
};


// char shifted_scancodeTranslator[NUM_SCANCODES] = // only putting alphabet and numbers, mapping all other keys to show nothing (0x0)
// // 83 is all entries when shift or caps lock is pressed

// {0x0, 0x0, '!', '@', '#', '$', '%', '^', '&', '*',  //27 is escape character
//  '(', ')', '_', '+', 0x0, 0x0, 'Q', 'W', 'E', 'R',  //8 is backspace char, tab
//  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0x0, 0x0,  //13 is enter, ctrl
//  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', 
//  '\"','~', 0x0, '|','Z', 'X', 'C', 'V', 'B', 'N',   // Left shift
//  'M', '<', '>', '?', 0x0, '*', 0x0, ' ', 0x0, 0x0,  // Right shift, left alt, caps lock, F1
//  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  // F2, F3, F4, F5, F6, F7, F8, F9, F10, Numlock
//  0x0, '7', '8', '9', 0x0, '4', '5', '6', 0x0, '1',  // Scroll lock, keypad -, keypad +
//  '2', '3', '0' 
// };

/*
 * keyboard_init
 *   DESCRIPTION: initializes the keyboard
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void keyboard_init(){
    enable_irq(1);     //1 is keyboard irq number
    // left_shift_flag = 0;
    // right_shift_flag = 0;
    // caps_flag = 0;
    // ctrl_flag = 0;
}

/*
 * keyboard_handler
 *   DESCRIPTION: handler for keyboard
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints alphanumeric characters when keys are pressed
 */
void keyboard_handler(){
    uint8_t scan_code = inb(0x60); // port to receive scancodes
    // switch(scan_code) {
    //     case 0x0E: 
    //         // Backspace pressed
    //         break;
    //     case 0x0F:
    //         // Tab pressed
    //         for(int i = 0; i < 5; i++) {
    //             putc(' ');
    //         }
    //         break;
    //     case 0x1C: 
    //         // Enter Pressed
    //         putc('\n');
    //         break;
    //     case 0x1D:
    //         // Control pressed
    //         ctrl_flag = 1;
    //         break;
    //     case 0x2A:
    //         // Left Shift pressed
    //         left_shift_flag = 1;
    //         break;
    //     case 0x36: 
    //         // Right Shift pressed
    //         right_shift_flag = 1;
    //         break;
    //     case 0x3A:
    //         // Caps-lock pressed
    //         if(caps_flag) caps_flag = 0;
    //         else caps_flag = 1;
    //         break;
    //     case 0x9D:
    //         // Control released
    //         ctrl_flag = 0;
    //         break;
    //     case 0xAA:
    //         // Left Shift released
    //         left_shift_flag = 0;
    //         break;
    //     case 0xB6:
    //         // Right Shift released
    //         right_shift_flag = 0;
    //         break;
    // }
    // // If shifted character
    // if(left_shift_flag || right_shift_flag || caps_flag) {
    //     if((scan_code < NUM_SCANCODES) && (scancodeTranslator[scan_code] != 0x0)){     // 0x0 is not mapped entry
    //         putc(shifted_scancodeTranslator[scan_code]);
    //     }
    // }
    // If non shifted character
    if((scan_code < NUM_SCANCODES) && (scancodeTranslator[scan_code] != 0x0)){     // 0x0 is not mapped entry
        putc(scancodeTranslator[scan_code]);
    }

    send_eoi(1);
}


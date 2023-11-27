

#include "keyboard.h"
#include "i8259.h"
#include "lib.h"

#define TAB_SPACE   4
#define NUM_COLS    80
#define NUM_ROWS    25

uint8_t left_shift_flag;
uint8_t right_shift_flag;
uint8_t caps_flag;
uint8_t ctrl_flag;
uint8_t alt_flag;

// Scan code set 1
char scancodeTranslator[NUM_SCANCODES] =       // only putting alphabet and numbers for now, mapping all other keys to show nothing (0x0)
// 83 is all entries included lowercase and all numbers

{0x0, 0x0, '1', '2', '3', '4', '5', '6', '7', '8',  //27 is escape character
 '9', '0', '-', '=', 0x0, 0x0, 'q', 'w', 'e', 'r',  //8 is backspace char, tab
 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0x0, 0x0,  //13 is enter, ctrl
 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 
 '\'','`', 0x0, '\\','z', 'x', 'c', 'v', 'b', 'n',  // Left shift
 'm', ',', '.', '/', 0x0, '*', 0x0, ' ', 0x0, 0x0,  // Right shift, left alt, caps lock, F1
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  // F2, F3, F4, F5, F6, F7, F8, F9, F10, Numlock
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  // Scroll lock, keypad -, keypad +
 0x0, 0x0, 0x0 
};


char shifted_scancodeTranslator[NUM_SCANCODES] = // only putting alphabet and numbers, mapping all other keys to show nothing (0x0)
// 83 is all entries when shift or caps lock is pressed

{0x0, 0x0, '!', '@', '#', '$', '%', '^', '&', '*',  //27 is escape character
 '(', ')', '_', '+', 0x0, 0x0, 'Q', 'W', 'E', 'R',  //8 is backspace char, tab
 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0x0, 0x0,  //13 is enter, ctrl
 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', 
 '\"','~', 0x0, '|','Z', 'X', 'C', 'V', 'B', 'N',   // Left shift
 'M', '<', '>', '?', 0x0, '*', 0x0, ' ', 0x0, 0x0,  // Right shift, left alt, caps lock, F1
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  // F2, F3, F4, F5, F6, F7, F8, F9, F10, Numlock
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  // Scroll lock, keypad -, keypad +
 0x0, 0x0, 0x0 
};

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
    left_shift_flag = 0;
    right_shift_flag = 0;
    caps_flag = 0;
    ctrl_flag = 0;
    alt_flag = 0;
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
    int i;
    term_t* curr_term = &terminal[curr_terminal];

    switch(scan_code) {
        case 0x0E: 
            // Backspace pressed
            if(curr_term->enter_flag == 0 && curr_term->buf_i > 0) {
                curr_term->buf_i--;
                curr_term->key_buf[curr_term->buf_i] = 0;
                curr_term->scr_x--;
                putc(' ');
                curr_term->scr_x--;
                update_cursor();
            }
            break;
        case 0x0F:
            // Tab pressed
            if(curr_term->enter_flag == 0) {
                for(i = 0; i < TAB_SPACE; i++) {
                    if(curr_term->buf_i == 127) break;
                    curr_term->key_buf[curr_term->buf_i] = ' ';
                    curr_term->buf_i++;
                    putc(' ');
                }   
            }
            break;
        case 0x1C: 
            // Enter Pressed
            if(curr_term->enter_flag == 0) {
                curr_term->key_buf[curr_term->buf_i] = '\n';
                curr_term->buf_i = 128;
                curr_term->enter_flag = 1;
                // scroll_down();
                putc('\n');
            }
            break;
        case 0x1D:
            // Control pressed
            ctrl_flag = 1;
            break;
        case 0x2A:
            // Left Shift pressed
            left_shift_flag = 1;
            break;
        case 0x36: 
            // Right Shift pressed
            right_shift_flag = 1;
            break;
        case 0x3A:
            // Caps-lock pressed
            if(caps_flag) caps_flag = 0;
            else caps_flag = 1;
            break;
        case 0x38:
            // alt pressed
            alt_flag = 1;
            break;
        case 0x3B: 
            // F1 pressed
            if(alt_flag == 1) {
                // //Switch To terminal 0
                // if(terminal[0].in_use == 0) {
                //     term_init(0, 0, 0);
                // }
                change_terminal(0);
            }
            break;
        case 0x3C:
            // F2 pressed
            if(alt_flag == 1) {
                // //Switch To terminal 1
                // if(terminal[1].in_use == 0) {
                //     term_init(1, 0, 0);
                // }
                change_terminal(1);
            }
            break;
        case 0x3D:
            // F3 pressed
            if(alt_flag == 1) {
                // //Switch to terminal 2
                // if(terminal[2].in_use == 0) {
                //     term_init(2, 0, 0);
                // }
                change_terminal(2);
            }
        case 0x9D:
            // Control released
            ctrl_flag = 0;
            break;
        case 0xAA:
            // Left Shift released
            left_shift_flag = 0;
            break;
        case 0xB6:
            // Right Shift released
            right_shift_flag = 0;
            break;
        case 0xB8:
            // alt released
            alt_flag = 0;
            break;
        default:
            // If ctrl+l or ctrl+L
            if(ctrl_flag && scancodeTranslator[scan_code] == 'l') {
                clear();
                curr_term->scr_x = 0;
                curr_term->scr_y = 0;
                update_cursor();
            }
            else if(curr_term->buf_i < keyboard_buf_size-1 && curr_term->enter_flag == 0) {
            // If shifted character
            if((left_shift_flag || right_shift_flag) != caps_flag) {
                if((scan_code < NUM_SCANCODES) && (scancodeTranslator[scan_code] != 0x0)){     // 0x0 is not mapped entry
                    curr_term->key_buf[curr_term->buf_i] = shifted_scancodeTranslator[scan_code];
                    curr_term->buf_i++;
                    putc(shifted_scancodeTranslator[scan_code]);
                }
            }
            // If non shifted character
            else if((scan_code < NUM_SCANCODES) && (scancodeTranslator[scan_code] != 0x0)){     // 0x0 is not mapped entry
                curr_term->key_buf[curr_term->buf_i] = scancodeTranslator[scan_code];
                curr_term->buf_i++;
                putc(scancodeTranslator[scan_code]);
            }
            }
        }

    send_eoi(1);
}


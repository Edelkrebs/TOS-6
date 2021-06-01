#include <driver/keyboard.h>
#include <driver/screen.h>
#include <debug.h>
#include <pic.h>

uint8_t pressed_keys[255];

uint8_t shift = 0;
uint8_t control = 0;
uint8_t capslock = 0;
uint8_t alt = 0;
uint8_t scroll_lock = 0;
uint8_t num_lock = 0;

__attribute__((unused)) static const uint8_t ascii_capslock[] = {
	'\0', '\e', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', '\0', 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', '\0', '\\', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', ',', '.', '/', '\0', '\0', '\0', ' '
};

__attribute__((unused)) static const uint8_t ascii_shift[] = {
	'\0', '\e', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '\0', 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', '\0', '|', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?', '\0', '\0', '\0', ' '
};

__attribute__((unused)) static const uint8_t ascii_shift_capslock[] = {
	'\0', '\e', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', '\0', 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '~', '\0', '|', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', '<', '>', '?', '\0', '\0', '\0', ' '
};

__attribute__((unused)) static const uint8_t ascii_nomod[] = {
	'\0', '\e', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0', 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', '\0', '\0', '\0', ' '
};

void keyboard_init(){
}

void process_scancode(uint8_t scancode){
	switch(scancode){
		case 0x2A:{
			shift = 1;
			return;
		}case 0x1D:{
			control = 1;
			return;
		}case 0x38:{
			alt = 1;
			return;
		}case 0x3A:{
			capslock = !capslock;
			return;
		}case 0x45:{
			num_lock = 1;
			return;
		}case 0x46:{
			scroll_lock = 1;
			return;
		}
			
		case 0x9D:{
			control = 0;
			return;
		}case 0xAA:{
			shift = 0;
			return;
		}case 0xB8:{
			alt = 0;
			return;
		}case 0xC5:{
			num_lock = 0;
			return;
		}case 0xC6:{
			scroll_lock = 0;
			return;
		}
	}

	if(!(scancode & 0x80)){
		if(shift && capslock){
			pressed_keys[ascii_shift_capslock[scancode]] = 1;
			return;
		}else if(shift || capslock){
			pressed_keys[ascii_shift[scancode]] = 1;
			return;
		}else{
			pressed_keys[ascii_nomod[scancode]] = 1;
			return;
		}
	}else{
		if(shift && capslock){
			pressed_keys[ascii_shift_capslock[scancode]] = 0;
			return;
		}else if(shift || capslock){
			pressed_keys[ascii_shift[scancode]] = 0;
			return;
		}else{
			pressed_keys[ascii_nomod[scancode]] = 0;
			return;
		}
	}
}
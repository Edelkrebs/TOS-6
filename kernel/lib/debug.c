#include <driver/vga_text.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#ifndef __FRAMEBUFFER_PRESENT
static inline uint16_t* coords_to_address(uint32_t row, uint32_t column){
	return (uint16_t*) 0xb8000 + (row * height + column);
}

static inline uint16_t generate_entry(char character, uint8_t color){
	return (color << 8) | character;
}

void printk(const char* str, uint32_t r, uint32_t col){

	if(!str) return;

	row = r;
	column = col;
	for(size_t i = 0; i <= strlen(str); i++){
		if(str[i] == '\n'){
			i++;
			row++;
			column = 0;
			continue;
		}
		*(coords_to_address(row, column)) = generate_entry(str[i], text_color_attrib);
		column++;
	}

	if(column > 0) column--;
	
}

void kputch(char c, uint32_t r, uint32_t col){
	if(c == '\n'){
		row++;
		column = 0;
		return;
	}
	*(coords_to_address(r, col)) = generate_entry(c, text_color_attrib);
	column++;	
}

void printhexln(uint64_t number){
	char* str =  "0x0000000000000000\0";
	uint64_t numbercpy = number;
	for(int i = 17; i > 1; i--){
		number = numbercpy;
		numbercpy = numbercpy >> 4;
		number &= 0xF;
		if(number > 9) str[i] = number + 'A' - 10;
		else str[i] = number + '0';
	}
	printk(str, row, column);
	kputch('\n', row, column);
}

void printhex(uint64_t number){
	char* str =  "0x0000000000000000\0";
	uint64_t numbercpy = number;
	for(int i = 17; i > 1; i--){
		number = numbercpy;
		numbercpy = numbercpy >> 4;
		number &= 0xF;
		if(number > 9) str[i] = number + 'A' - 10;
		else str[i] = number + '0';
	}
	printk(str, row, column);
}

void putch(char c){
	kputch(c, row, column);
}

void println(const char* str){
	printk(str, row, column);
	putch('\n');
}

void print(const char* str){
	printk(str, row, column);
}

void warn(const char* str){
	uint32_t t_color = text_color;
	set_text_color(VGA_LIGHT_CYAN);
	printk("[WARNING] ", row, column);
	set_text_color(t_color);
	printk(str, row, column);
}

void error(const char* str){
	uint32_t t_color = text_color;
	set_text_color(VGA_RED);
	printk("[ERROR] ", row, column);
	set_text_color(VGA_LIGHT_RED);
	printk(str, row, column);
	set_text_color(t_color);
}

void panic(const char* message){
	error("KERNEL PANICED! With message: ");
	set_text_color(VGA_RED);
	println(message);
	set_text_color(VGA_WHITE);

	while(1);
}

void cls(){
	for(int i = 0; i <= width * height; i++){
		*((uint16_t*) 0xb8000 + i) = generate_entry(' ', text_color_attrib);
	}
	row = 0;
	column = 0;
}
#else


#endif
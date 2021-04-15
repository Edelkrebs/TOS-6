#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdint.h>

typedef enum{
	OK,
	SUCCESS,
	WARNING,
	ERROR
} LOG_TYPE;

extern uint8_t font_width;
extern uint8_t font_height;

void cls();

void putch(char c);
void print(const char* str);
void println(const char* str);
void log(const char* str, LOG_TYPE type);

void printhex(uint64_t number);
void printhexln(uint64_t number);

void warn(const char* str);
void error(const char* str);
void panic(const char* message);


static inline void assert(uint32_t condition, const char* message){
	if(condition) panic(message);
}

#endif

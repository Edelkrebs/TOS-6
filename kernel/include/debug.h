#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdint.h>

void putch(char c);
void printhex(uint64_t number);
void printhexln(uint64_t number);

void warn(const char* str);
void error(const char* str);
void panic(const char* message);

void cls();
void print(const char* str);
void println(const char* str);

static inline void assert(uint32_t condition, const char* message){
	if(condition) panic(message);
}

#endif

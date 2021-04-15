#include <mm/memory.h>

void memcpy(uint8_t* src, uint8_t* dest, uint64_t count){
	for(uint64_t i = 0; i < count; i++){
		src[i] = dest[i];
	}
}

uint8_t memcmp(uint8_t* src, uint8_t* dest, uint64_t count){
	for(uint64_t i = 0; i < count; i++){
		if(src[i] != dest[i]){
			return 1;
		}
	}
	return 0;
}
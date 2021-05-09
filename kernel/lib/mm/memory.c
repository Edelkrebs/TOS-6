#include <mm/memory.h>

void memcpy(void* src, void* dest, uint64_t count){
	for(uint64_t i = 0; i < count; i++){
		((uint8_t*)src)[i] = ((uint8_t*)dest)[i];
	}
}

uint8_t memcmp(void* src, void* dest, uint64_t count){
	for(uint64_t i = 0; i < count; i++){
		if(((uint8_t*)src)[i] != ((uint8_t*)dest)[i]){
			return 1;
		}
	}
	return 0;
}

void memset(void* src, uint8_t value, uint64_t count){
	for(uint64_t i = 0; i < count; i++){
		((uint8_t*)src)[i] = value;
	}
}
#include <stdint.h>
#include <stddef.h>
#include <stivale2.h>
#include <driver/screen.h>

void kmain(struct stivale2_struct *stivale2_struct) {

	stivale2Init(stivale2_struct);

	while(1) asm("hlt");

}
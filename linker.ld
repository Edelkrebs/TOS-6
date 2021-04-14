ENTRY(kmain)
 
SECTIONS
{
    . = 0xffffffff80200000;
 	_kernel_start = .;
    .stivale2hdr : {
        KEEP(*(.stivale2hdr))
    }
 
    /* Then let's place all the other traditional executable sections afterwards. */
    .text : {
        *(.text*)
    }
 
    .rodata : {
        *(.rodata*)
    }
 
    .data : {
        *(.data*)
    }
 
    .bss : {
        *(COMMON)
        *(.bss*)
    }

    _kernel_end = .;
}
ENTRY(_start)

SECTIONS
{
    . = 0x40000000;
    
    .text : {
        *(.text.boot)
        *(.text)
        *(.text*)
    }
    
    . = ALIGN(4096);
    .rodata : {
        *(.rodata*)
    }
    
    .data : {
        *(.data*)
    }
    
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(16);
        __bss_end = .;
    }
}
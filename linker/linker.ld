ENTRY(_start)

SECTIONS
{
    . = 0x40000000;
    __kernel_start = .;
    
    .text : {
        __text_start = .;
        *(.text.boot)
        *(.text)
        *(.text*)
        . = ALIGN(16);
        __text_end = .;
    }
    
    . = ALIGN(4096);
    .rodata : {
        __rodata_start = .;
        *(.rodata*)
        . = ALIGN(16);
        __rodata_end = .;
    }
    
    . = ALIGN(4096);
    .data : {
        __data_start = .;
        *(.data*)
        . = ALIGN(16);
        __data_end = .;
    }
    
    . = ALIGN(4096);
    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4096);
        __bss_end = .;
    }

    __kernel_end = .;
}

/*******************************/
/** Entry Point for Processor **/
/*******************************/
ENTRY(Reset_handler)

/******************/
/** Top of Stack **/
/******************/
__estack = 0x20000000 + 96K;

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
    .isr_vector : 
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    }> FLASH

    .test :
    {
        . = ALIGN(4);
        *(.text)
        *(.rodata)
        . = ALIGN(4);
        __etext = .;
    }> FLASH

    .data :
    {
        . = ALIGN(4);
        __sdata = .;
        *(.data)
        . = ALIGN(4);
        __edata = .;
    }> SRAM AT> FLASH

    .bss :
    {
        . = ALIGN(4);
        __sbss = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        __ebss = .;
    }> SRAM
}


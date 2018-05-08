__stack_size = 1024;

MEMORY {
    flash(rx) : org = 0x08000000, len = 512k
    sram(rwx) : org = 0x20000000, len = 96k
}

SECTIONS {
    .text : {
        KEEP(*(vectors))

        *(.text .rodata .rodata.* .gnu.linkonce.r.*)
    } > flash

    .data : {
        __data_init_start = LOADADDR(.data);
        __data_start = .;
        *(.data .data.* .gnu.linkonce.d.*)
        __data_end = .;
    } > sram AT > flash

    .bss : {
        __bss_start = .;
        *(.bss .bss.* .gnu.linkonce.b.*)
        __bss_end = .;
    } > sram

    .stack : {
        . = ALIGN(4);
        __stack_bottom = .;
        . += (__stack_size);
        __stack_top = .;
    } > sram
}

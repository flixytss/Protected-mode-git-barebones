ENTRY(_start)

SECTIONS {
    . = 0x10000;

    .text.start : {
        *(.text.start)
    }

    .text : {
        *(.text*)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss COMMON)
    }
}

PHDRS {
    null PT_NULL;
    code PT_LOAD;
}

SECTIONS {
    . = 0x7c00;
    .boot : { *(.boot) } :code
    .text : { *(.text) } :code
    .data : { *(.data) }
    /DISCARD/ : { *(.eh_frame) }
}

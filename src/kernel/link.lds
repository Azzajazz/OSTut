PHDRS {
    null PT_NULL;
    code PT_LOAD;
}

SECTIONS {
    . = 0x500;
    .text : { *(.text) } :code
    .data : { *(.data) }
    /DISCARD/ : { *(.eh_frame) }
}

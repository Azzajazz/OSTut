PHDRS {
    null PT_NULL;
    code PT_LOAD;
}

SECTIONS {
    . = 0x8000;
    .text : { *(.text) } :code
    .data : { *(.data) }
    /DISCARD/ : { *(.eh_frame) }
}

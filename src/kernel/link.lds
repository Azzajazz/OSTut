PHDRS {
    null PT_NULL;
    code PT_LOAD;
}

SECTIONS {
    . = 0x8000;
    .entry : { *(.entry) } :code
    .text : { *(.text) } :code
    .data : { *(.data) }
    /DISCARD/ : { *(.eh_frame) }
}

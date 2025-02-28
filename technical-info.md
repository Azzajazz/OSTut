# Technical Information

## Memory map

### Low memory

|start     |end (exc)  |size (B)|contents     |preserve?  |
|----------|-----------|--------|-------------|:---------:|
|0x00000000|0x000003ff |1024    |Real Mode IVT|RM: Y PM: N|
|0x00000400|0x000004ff |256     |BDA          |RM: Y PM: N|
|0x00000500|0x00007bff |30464   |TBD          |RM: N PM: N|
|0x00007c00|0x00007dff |512     |Boot sector  |RM: N PM: N|
|0x00007e00|0x0009fbff |622592  |TBD          |RM: N PM: N|
|0x0009fc00|0x0009ffff |1024    |EBDA         |RM: Y PM: Y|
|0x000a0000|0x000bffff |131072  |Video memory |RM: Y PM: Y|
|0x000c0000|0x000c7fff |32768   |Video BIOS   |RM: Y PM: Y|
|0x000c8000|0x000effff?|163840? |BIOS Ext     |RM: Y PM: Y|
|0x000f0000|0x000fffff |65536   |MBoard BIOS  |RM: Y PM: Y|

### High memory

Unclear. This is not well defined and probably needs to be mapped out by the bootloader?

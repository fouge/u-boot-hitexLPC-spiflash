OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm",
       "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(_start)


MEMORY
{
 SPIFI (rx): ORIGIN = (0x14000000), LENGTH = ((16 * 1024 * 1024) - 0x10)
 Local_SRAM128 (rwx): ORIGIN = 0x10000000, LENGTH = (128*1024)
 RAM (rw): ORIGIN = 0x20000000, LENGTH = (32 * 1024)
 RAM_BUF (r): ORIGIN = 0x20000000 + LENGTH(RAM), LENGTH = (1 * 1024)
 MALLOC (r): ORIGIN = 0x20000000 + LENGTH(RAM) + LENGTH(RAM_BUF), LENGTH = (27 * 1024)
 STACK (r): ORIGIN = 0x20000000 + LENGTH(RAM) + LENGTH(RAM_BUF) + LENGTH(MALLOC), LENGTH = (4 * 1024)
}
SECTIONS
{
 .vectors :
 {
  *(.vectors)
  
 } >Local_SRAM128 AT>SPIFI
 
 .lpc18xx_image_top :
 {
   /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        LONG(LOADADDR(.vectors));
        LONG(    ADDR(.vectors));
        LONG(  SIZEOF(.vectors));
        LONG(LOADADDR(.text));
        LONG(    ADDR(.text));
        LONG(  SIZEOF(.text));
        LONG(LOADADDR(.constdata));
        LONG(    ADDR(.constdata));
        LONG(  SIZEOF(.constdata));
        LONG(LOADADDR(.u_boot_cmd));
        LONG(    ADDR(.u_boot_cmd));
        LONG(  SIZEOF(.u_boot_cmd));
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        __section_table_end = . ;
    /* Global Section Table end */
    
  *(.lpc18xx_image_top_text)
  *(.lpc18xx_image_top_data)
 } >SPIFI
 .text :
 {
  _text_start = .;
  *(.text)
  *(.rodata*)
  _text_end = .;
 } >Local_SRAM128 AT>SPIFI
 .data :
 {
  _data_start = .;
  _data_lma_start = LOADADDR(.data);
  *(.data)
  . = ALIGN(4);
  *(.ramcode)
  _data_end = .;
 } >RAM AT>SPIFI
 .u_boot_cmd :
 {
  . = ALIGN(4);
  __u_boot_cmd_start = .;
  *(.u_boot_cmd)
  __u_boot_cmd_end = .;
 } >Local_SRAM128 AT>SPIFI
 .bss :
 {
  _bss_start = .;
  *(.bss)
  *(COMMON)
  _bss_end = .;
 } >RAM
 .stack :
 {
  *(.stack)
 } >STACK
 /* Has to be removed to keep .debug_* sections used for debugging
  * /DISCARD/ :
 {
  *(*)
 } */
}
_mem_nvm_base = ORIGIN(SPIFI);
_mem_nvm_size = LENGTH(SPIFI);
_mem_ram_base = ORIGIN(RAM);
_mem_ram_size = LENGTH(RAM);
_mem_ram_buf_base = ORIGIN(RAM_BUF);
_mem_ram_buf_size = LENGTH(RAM_BUF);
_mem_malloc_base = ORIGIN(MALLOC);
_mem_malloc_size = LENGTH(MALLOC);
_mem_stack_base = ORIGIN(STACK);
_mem_stack_size = LENGTH(STACK);
_mem_stack_end = ORIGIN(STACK) + LENGTH(STACK);
/* K22F Linker Script for MK22FN512VLH12 */

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 512K
  RAM   (rwx) : ORIGIN = 0x1FFF0000, LENGTH = 128K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

ENTRY(Reset_Handler)

SECTIONS
{
  /* Vector table at 0x0 */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } > FLASH

  /* Pad to 0x400 for Flash Configuration Field */
  .fcf_pad :
  {
    . = ALIGN(4);
    FILL(0xFF);
    . = ORIGIN(FLASH) + 0x400;
  } > FLASH

  /* Flash Configuration Field - MUST be at 0x400 */
  .flashconfig 0x400 :
  {
    KEEP(*(.flashconfig))
  } > FLASH

  /* Verify FCF is at correct address */
  ASSERT(. == 0x410, "ERROR: Flash Config Field not at 0x400!")

  /* Code */
  .text :
  {
    . = ALIGN(4);
    *(.text*)
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(4);
    _etext = .;
  } > FLASH

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH

  .ARM.exidx :
  {
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
  } > FLASH

  .eh_frame :
  {
    KEEP(*(.eh_frame))
  } > FLASH

  .eh_frame_hdr :
  {
    *(.eh_frame_hdr)
  } > FLASH

  /* Initialized data */
  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM AT > FLASH

  /* Uninitialized data */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } > RAM

  . = ALIGN(8);
  _end = .;
  end = .;
  PROVIDE(__heap_start = _end);
}

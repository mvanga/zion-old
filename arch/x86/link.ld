/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

ENTRY(start)

SECTIONS
{
	__text_start = .;
	.text 0x100000 : {
		*(.text)
	}
	. = ALIGN(4096);
	__text_end = .;

	. = ALIGN(16);
	__init_begin = .;
	.text.init : {
		*(.module0.init);
		*(.module1.init);
		*(.module2.init);
	}
	__init_end = .;

	__exit_begin = .;
	.text.exit : {   
		*(.module0.exit);
		*(.module1.exit);
		*(.module2.exit);
	}
	__exit_end = .;

	__data_start = .;
	.data : {
		*(.data)
		*(.rodata)
	}
	. = ALIGN(4096);
	__data_end = .;

	__bss_start = .;
	.bss : {
		bss = .;
		*(.bss)
	}
	. = ALIGN(4096);
	__bss_end = .;
}

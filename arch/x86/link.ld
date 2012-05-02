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
		__early_init_begin = .;
		*(.module0.init);
		__early_init_end = .;
		__arch_init_begin = .;
		*(.module1.init);
		__arch_init_end = .;
		__core_init_begin = .;
		*(.module2.init);
		__core_init_end = .;
		__module_init_begin = .;
		*(.module3.init);
		__module_init_end = .;
	}
	__init_end = .;

	__exit_begin = .;
	.text.exit : {   
		__arch_exit_begin = .;
		*(.module1.exit);
		__arch_exit_end = .;
		__core_exit_begin = .;
		*(.module2.exit);
		__core_exit_end = .;
		__module_exit_begin = .;
		*(.module3.exit);
		__module_exit_end = .;
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
	__alloc_start = .;
}

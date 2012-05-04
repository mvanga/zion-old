ENTRY(start)

SECTIONS
{
	. = 0x100000;
	.setup : {
		*(.setup)
	}

	. += 0xC0000000;

	.text : AT(ADDR(.text) - 0xC0000000) {
		*(.text)
		__text_end = .;
	}

	.text.init : AT(ADDR(.text.init) - 0xC0000000) {
		__init_begin = .;
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
		__init_end = .;
	}

	.text.exit : AT(ADDR(.text.exit) - 0xC0000000) {   
		__exit_begin = .;
		__arch_exit_begin = .;
		*(.module1.exit);
		__arch_exit_end = .;
		__core_exit_begin = .;
		*(.module2.exit);
		__core_exit_end = .;
		__module_exit_begin = .;
		*(.module3.exit);
		__module_exit_end = .;
		__exit_end = .;
	}

	.data ALIGN(4096) : AT(ADDR(.data) - 0xC0000000) {
		*(.data)
		*(.rodata)
		__data_end = .;
	}
	. = ALIGN(4096);

	.bss ALIGN(4096) : AT(ADDR(.bss) - 0xC0000000) {
		bss = .;
		*(.bss)
		__bss_end = .;
	}
	. = ALIGN(4096);
	__alloc_start_phys = . - 0xC0000000;
	__alloc_start = .;
}

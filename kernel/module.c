#include <zion/module.h>

extern init_t __init_begin;
extern init_t __init_end;

extern init_t __early_init_begin;
extern init_t __early_init_end;

extern init_t __arch_init_begin;
extern init_t __arch_init_end;

extern init_t __core_init_begin;
extern init_t __core_init_end;

extern init_t __module_init_begin;
extern init_t __module_init_end;

extern exit_t __exit_begin;
extern exit_t __exit_end;

extern exit_t __early_exit_begin;
extern exit_t __early_exit_end;

extern exit_t __arch_exit_begin;
extern exit_t __arch_exit_end;

extern exit_t __core_exit_begin;
extern exit_t __core_exit_end;

extern exit_t __module_exit_begin;
extern exit_t __module_exit_end;

void do_initcalls_all(void)
{
	init_t *call;
	call = &__init_begin;
	while (call < &__init_end) {
		(*call)();
		call++;
	}
}

void do_exitcalls_all(void)
{
	exit_t *call;
	/* Deinitialize in reverse */
	call = &__exit_end;
	call--;
	while (call >= &__exit_begin) {
		(*call)();
		call--;
	}
}

void do_initcalls_early(void)
{
	init_t *call;
	call = &__early_init_begin;
	while (call < &__early_init_end) {
		(*call)();
		call++;
	}
}

void do_initcalls_arch(void)
{
	init_t *call;
	call = &__arch_init_begin;
	while (call < &__arch_init_end) {
		(*call)();
		call++;
	}
}

void do_exitcalls_arch(void)
{
	exit_t *call;
	/* Deinitialize in reverse */
	call = &__arch_exit_end;
	call--;
	while (call >= &__arch_exit_begin) {
		(*call)();
		call--;
	}
}

void do_initcalls_core(void)
{
	init_t *call;
	call = &__core_init_begin;
	while (call < &__core_init_end) {
		(*call)();
		call++;
	}
}

void do_exitcalls_core(void)
{
	exit_t *call;
	/* Deinitialize in reverse */
	call = &__core_exit_end;
	call--;
	while (call >= &__core_exit_begin) {
		(*call)();
		call--;
	}
}

void do_initcalls_module(void)
{
	init_t *call;
	call = &__module_init_begin;
	while (call < &__module_init_end) {
		(*call)();
		call++;
	}
}

void do_exitcalls_module(void)
{
	exit_t *call;
	/* Deinitialize in reverse */
	call = &__module_exit_end;
	call--;
	while (call >= &__module_exit_begin) {
		(*call)();
		call--;
	}
}

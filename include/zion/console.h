#ifndef ZION_CONSOLE_H
#define ZION_CONSOLE_H

#include <zion/list.h>

struct console {
	int index;
	char name[16];
	void (*setup)(struct console *);
	void (*write)(struct console *, const char *, unsigned);
	int (*read)(struct console *, char *, unsigned);
	struct list_head list;
};

extern struct console *early;

int console_register(struct console *);
void console_unregister(struct console *);

void console_setup(struct console *);
void console_write(struct console *, const char *, unsigned);
int console_read(struct console *, char *, unsigned);

struct console *console_get(int);

#endif

#include <zion/console.h>
#include <zion/list.h>
#include <zion/stdio.h>

static LIST_HEAD(consoles);

struct console *early;

int console_register(struct console *c)
{
	if (!c)
		return -1;
	if (c->setup)
		c->setup(c);
	if (c->index == 0)
		early = c;
	list_add(&c->list, &consoles);
	return 0;
}

void console_unregister(struct console *c)
{
	if (!c)
		return;
	list_del(&c->list);
}

void console_setup(struct console *c)
{
	if (!c || !c->setup)
		return;
	c->setup(c);
}

void console_write(struct console *c, const char *buf, unsigned len)
{
	if (!c || !c->write)
		return;
	c->write(c, buf, len);
}

int console_read(struct console *c, char *buf, unsigned len)
{
	if (!c || !c->read)
		return -1;
	return c->read(c, buf, len);
}

struct console *console_get(int idx)
{
	struct console *c;

	list_for_each_entry(c, &consoles, list) {
		if (c->index == idx)
			return c;
	}
	return NULL;
}

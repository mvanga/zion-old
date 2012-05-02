#include <zion/module.h>
#include <zion/console.h>
#include <zion/io.h>

/* Simple VGA output */
#define VGABASE		(0xb8000)

static int max_ypos = 25;
static int max_xpos = 80;
static int current_ypos = 0;
static int current_xpos = 0;

static void early_printk_move_cursor(void)
{
	uint16_t cursor = current_ypos * 80 + current_xpos;
	outb(14, 0x3d4);
	outb(cursor >> 8, 0x3d5);
	outb(15, 0x3d4);
	outb(cursor, 0x3d5);
}

static void early_printk_setup(struct console *c)
{
	int i;

	for (i = 0; i < 80*25; i++)
		console_write(c, " ", 1);
	current_ypos = 0;
	current_xpos = 0;
	early_printk_move_cursor();
}

static void early_printk_write(struct console *con __attribute__((unused)),
	const char *str, unsigned n)
{
	char c;
	int  i, k, j;

	while ((c = *str++) != '\0' && n-- > 0) {
		if (current_ypos >= max_ypos) {
			/* scroll 1 line up */
			for (k = 1, j = 0; k < max_ypos; k++, j++) {
				for (i = 0; i < max_xpos; i++) {
					writew(readw(VGABASE+2*(max_xpos*k+i)),
							VGABASE + 2*(max_xpos*j + i));
				}
			}
			for (i = 0; i < max_xpos; i++)
				writew(0x720, VGABASE + 2*(max_xpos*j + i));
			current_ypos = max_ypos-1;
		}
		if (c == '\n') {
			current_xpos = 0;
			current_ypos++;
		} else if (c != '\r')  {
			writew(((0x7 << 8) | (unsigned short) c),
					VGABASE + 2*(max_xpos*current_ypos +
						current_xpos++));
			if (current_xpos >= max_xpos) {
				current_xpos = 0;
				current_ypos++;
			}
		}
		early_printk_move_cursor();
	}
}


struct console early_printk = {
	.index = 0,
	.name = "early_printk",
	.setup = early_printk_setup,
	.write = early_printk_write,
};

int early_printk_init(void)
{
	return console_register(&early_printk);
}

early_init(early_printk_init);

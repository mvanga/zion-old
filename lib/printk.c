#include <zion/console.h>
#include <zion/stdio.h>
#include <zion/stdarg.h>

#define PRINTK_BUFSIZE 128

static char printk_buf[PRINTK_BUFSIZE];

int getc(void)
{
	char c;

	return console_read(early, (char *)&c, 1);
}

char *gets(char *s, int size)
{
	int i = 0;
	while (i++ < size) {
		s[i-1] = getc();
		if (s[i-1] == '\n')
			break;
	}
	if (i > 0 && s[i-1] == '\n')
		s[i-1] = '\0';
	else
		s[i] = '\0';
	return s;
}

void putc(int c)
{
	console_write(early, (char *)&c, 1);
}

void puts(char *s)
{
	while (*s)
		putc(*s++);
}

int vprintk(const char *fmt, va_list args)
{
	int ret;

	ret = vsprintf(printk_buf, fmt, args);
	puts(printk_buf);
	return ret;
}

int printk(const char *fmt, ...)
{
	va_list args;
	int r;

	va_start(args, fmt);
	r = vprintk(fmt, args);
	va_end(args);

	return r;
}

int sprintf(char * buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	return i;
}


NAME=zion
ARCH=x86
CONFIG_ARCH_X86=y
CONFIG_ARCH_X86_TIMER=y

AS=as

obj-$(CONFIG_ARCH_X86) += arch/x86/boot.o arch/x86/init.o arch/x86/early_printk.o arch/x86/gdt.o arch/x86/idt.o arch/x86/segment.o arch/x86/irq.o
obj-$(CONFIG_ARCH_X86_TIMER) += arch/x86/timer.o
obj-y += kernel/main.o kernel/console.o kernel/timer.o
obj-y += lib/atoi.o lib/ctype.o lib/printk.o lib/rand.o lib/string.o lib/strtol.o lib/vsprintf.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -I./arch/$(ARCH)/include -I./include -Wall -Wextra -ggdb
LDFLAGS=-Tarch/$(ARCH)/link.ld
ASFLAGS=

all: $(obj-y) link bin

clean:
	rm -rf $(obj-y) $(NAME) $(NAME).bin

link:
	ld $(LDFLAGS) -o $(NAME) $(obj-y)

bin:
	objcopy -Obinary $(NAME) $(NAME).bin

run: clean all
	qemu -kernel $(NAME)

NAME=zion
ARCH=x86
CONFIG_ARCH_X86=y

obj-$(CONFIG_ARCH_X86) += arch/x86/boot.o arch/x86/init.o arch/x86/early_printk.o
obj-y += kernel/main.o kernel/console.o
obj-y += lib/atoi.o lib/ctype.o lib/printk.o lib/rand.o lib/string.o lib/strtol.o lib/vsprintf.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -I./arch/$(ARCH)/include -I./include
LDFLAGS=-Tarch/$(ARCH)/link.ld
ASFLAGS=

all: $(obj-y) link bin

clean:
	rm -rf $(obj-y) $(NAME) $(NAME).bin

link:
	ld $(LDFLAGS) -o $(NAME) $(obj-y)

bin:
	objcopy -Obinary $(NAME) $(NAME).bin

run:
	qemu -kernel $(NAME)

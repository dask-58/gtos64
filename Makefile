CC = clang
AS = clang
LD = ld.lld
OBJCOPY = /opt/homebrew/opt/llvm/bin/llvm-objcopy

CFLAGS = -target aarch64-none-elf -ffreestanding -nostdlib -Wall -Wextra -O2 -Iinclude
ASFLAGS = -target aarch64-none-elf
LDFLAGS = -T linker/linker.ld

C_SOURCES = $(wildcard kernel/*.c)
ASM_SOURCES = $(wildcard boot/*.s)
OBJECTS = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)

all: kernel.elf

kernel.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

run: kernel.elf
	qemu-system-aarch64 -M virt -cpu cortex-a53 -kernel kernel.elf -nographic

debug: kernel.elf
	qemu-system-aarch64 -M virt -cpu cortex-a53 -kernel kernel.elf -nographic -s -S

clean:
	rm -f kernel.elf $(OBJECTS)

.PHONY: all run debug clean
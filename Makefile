CC = clang
AS = clang
LD = ld.lld
OBJCOPY = /opt/homebrew/opt/llvm/bin/llvm-objcopy
QEMU = qemu-system-aarch64

CFLAGS = -target aarch64-none-elf -ffreestanding -nostdlib -Wall -Wextra -O2 -Iinclude
ASFLAGS = -target aarch64-none-elf
LDFLAGS = -T linker/linker.ld
QEMU_FLAGS = -M virt -cpu cortex-a53 -kernel kernel.elf -nographic
QEMU_NET_FLAGS = -netdev user,id=net0 -device virtio-net-device,netdev=net0,bus=virtio-mmio-bus.0,mac=52:54:00:12:34:56

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
	$(QEMU) $(QEMU_FLAGS) $(QEMU_NET_FLAGS)

debug: kernel.elf
	$(QEMU) $(QEMU_FLAGS) $(QEMU_NET_FLAGS) -s -S

clean:
	rm -f kernel.elf $(OBJECTS)

.PHONY: all run debug clean

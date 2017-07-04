.POSIX:

# Constants

BOOT_PATH ?= iso/boot
SRC_PATH ?= src

ELF_FILE ?= main.elf
IMG_FILE ?= main.img

OBJECTS := $(patsubst %.c,%.o,$(wildcard */*.c)) $(patsubst %.asm,%.o,$(wildcard */*.asm)) boot.o

LD_FLAGS := -melf_i386
GCC_FLAGS := -c -ggdb3 -Wextra -fno-builtin -fno-stack-protector -m32 -nostdlib -nostdinc -std=gnu99

NASM_RULE := nasm -felf -o

.PHONY: all clean debug qemu

# Default action
all: $(IMG_FILE)

# Make image file using linked ELF
$(IMG_FILE): $(ELF_FILE)
	cp '$<' '$(BOOT_PATH)'
	#grub-mkrescue -o '$@' iso
	grub-mkrescue -d /usr/lib/grub/i386-pc/ -o '$@' iso

# Link .o files
$(ELF_FILE): $(OBJECTS)
	ld -o '$(ELF_FILE)' $(LD_FLAGS) -Tlink.ld $^

# Compile .c files
$(SRC_PATH)/%.o: $(SRC_PATH)/%.c
	gcc $(GCC_FLAGS) -o '$@' '$<'

# Assemble .asm files
$(SRC_PATH)/%.o: $(SRC_PATH)/%.asm
	$(NASM_RULE) '$@' '$<'

# Assemble boot assembly file
boot.o: boot.asm
	$(NASM_RULE) '$@' '$<'

# Delete compiled objects
clean:
	rm -f $(OBJECTS) '$(SRC_PATH)'/*.o '$(BOOT_PATH)'/*.elf '$(ELF_FILE)' '$(IMG_FILE)' *.out

# Run QEMU using image
# Access monitor via "telnet localhost 14062"
qemu: $(IMG_FILE)
	qemu-system-i386 -hda '$<' -monitor telnet:localhost:14062,server,nowait


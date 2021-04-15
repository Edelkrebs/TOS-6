TARGET := kernel.elf
 
CC = x86_64-elf-gcc
 
CFLAGS = -Wall -Wextra -O2 -pipe
 
LDINTERNALFLAGS :=  \
	-Tlinker.ld \
	-nostdlib   \
	-shared     \
	-pie -fno-pic -fpie \
	-z max-page-size=0x1000
 
INTERNALCFLAGS  :=           \
	-I kernel/include    \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic -fpie       \
	-mno-80387           \
	-mno-mmx             \
	-mno-3dnow           \
	-mno-sse             \
	-mno-sse2            \
	-mno-red-zone		 \
	-Werror				 \
	-Wall
 
CFILES := $(shell find ./kernel -type f -name '*.c')
ASMFILES := $(shell find ./kernel -type f -name '*.asm')
OBJ    := $(CFILES:.c=.o) $(ASMFILES:.asm=.o)

		
run: image
	qemu-system-x86_64 -m 4G -no-reboot -no-shutdown -monitor stdio -d int image.hdd -enable-kvm

image: all
	touch image.hdd
	rm image.hdd
	
	dd if=/dev/zero bs=1M count=0 seek=64 of=image.hdd
	 
	parted -s image.hdd mklabel gpt
	 
	parted -s image.hdd mkpart primary 2048s 100%
	 
	echfs-utils -g -p0 image.hdd quick-format 512
	 
	echfs-utils -g -p0 image.hdd import limine.cfg limine.cfg
	echfs-utils -g -p0 image.hdd import kernel.elf kernel.elf
	  
	./limine/limine-install image.hdd
	echfs-utils -g -p0 image.hdd import limine/limine.sys limine.sys
 
all: $(TARGET)
 
$(TARGET): $(OBJ)
	$(CC) $(LDINTERNALFLAGS) $(OBJ) -o $@
 
%.o: %.c
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

%.o: %.asm
	nasm -felf64 $< -o $@
 
limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v2.0-branch-binary --depth=1
		 
	make -C limine

clean:
	rm -rf $(TARGET) $(OBJ)
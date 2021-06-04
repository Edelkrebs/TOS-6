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
	-Wall				 \
	-Wextra
 
CFILES := $(shell find ./kernel -type f -name '*.c')
ASMFILES := $(shell find ./kernel -type f -name '*.asm')
OBJ    := $(CFILES:.c=.o) $(ASMFILES:.asm=.o)

		
run: image
	qemu-system-x86_64 -m 4G -no-reboot -no-shutdown -monitor stdio -d int image.hdd -enable-kvm -cpu host -smp 4 -M q35 -trace ahci*

run_uefi: uefi_img
	qemu-system-x86_64 -m 4G -no-reboot -no-shutdown -monitor stdio -d int image.hdd -enable-kvm -bios OVMF.fd -cpu host -smp 4 -M q35 -trace ahci*

image: all

	-rm image.hdd
	dd if=/dev/zero bs=1M count=0 seek=64 of=image.hdd
#	 
#	 
#	 
#	echfs-utils -g -p0 image.hdd quick-format 512
#	 
#	echfs-utils -g -p0 image.hdd import limine.cfg limine.cfg
#	echfs-utils -g -p0 image.hdd import kernel.elf kernel.elf
#	  
	parted image.hdd mklabel gpt
	rm -rf data/boot
	mkdir -p data/boot
	mkfs.ext2 image.hdd
	mount image.hdd data/boot
	cp -p limine.cfg data/boot/limine.cfg
	cp -p kernel.elf data/boot/kernel.elf
	cp -p limine/limine.sys data/boot/limine.sys
	./limine/limine-install image.hdd
	umount -f data
#	echfs-utils -g -p0 image.hdd import limine/limine.sys limine.sys

uefi_img: all
	rm -rf pack
	mkdir -p pack
	cp kernel.elf limine.cfg ./limine/limine.sys pack/
	mkdir -p pack/EFI/BOOT
	cp ./limine/BOOTX64.EFI pack/EFI/BOOT/
	chmod +x ./dir2fat32.sh
	./dir2fat32.sh -f image.hdd 64 pack
	./limine/limine-install image.hdd
 
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

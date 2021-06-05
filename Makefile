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
	sudo qemu-system-x86_64 -m 4G -no-reboot -no-shutdown -monitor stdio -d int image.hdd -enable-kvm -cpu host -smp 4 -M q35 -trace ahci*

run_uefi: uefi_img
	qemu-system-x86_64 -m 4G -no-reboot -no-shutdown -monitor stdio -d int image.hdd -enable-kvm -bios OVMF.fd -cpu host -smp 4 -M q35 -trace ahci*

image: all

	sudo chmod +rwx ./shitpiss_images.sh
	sudo ./shitpiss_images.sh

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

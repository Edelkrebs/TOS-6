	rm -f image.hdd
	dd if=/dev/zero bs=1M count=0 seek=64 of=image.hdd

	parted -s ./image.hdd mklabel msdos
	parted -s ./image.hdd mkpart primary 1 100%

	LOOPDEV=$(sudo losetup --show -Pf ./image.hdd)
	LOOPPART=$LOOPDEV"p1"

	sudo mkfs.ext2 $LOOPPART
	
	mkdir -p ./mount

	sudo mount $LOOPPART --target ./mount

	sudo mkdir ./mount/boot

	sudo cp kernel.elf ./mount/boot/kernel.elf
	sudo cp limine.cfg ./mount/boot/limine.cfg

	sudo cp ./limine/limine.sys ./mount/limine.sys

	sudo umount ./mount

	sudo losetup --detach $LOOPDEV

	rm -rf ./mount

	./limine/limine-install image.hdd
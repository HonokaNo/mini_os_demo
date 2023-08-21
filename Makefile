BOOT_SRC := boot.c lib.c
BOOT_HEADER = lib.h uefi.h

boot.efi: $(BOOT_HEADER) $(BOOT_SRC)
	clang -O0 -Wall -Wextra -mabi=sysv -nostdlib -mno-sse -fno-builtin -fshort-wchar -target x86_64-pc-win32-coff -fuse-ld=lld -Wl,"/entry:BootMain" -Wl,"/SUBSYSTEM:efi_application" -mno-stack-arg-probe -o $@ $(BOOT_SRC)

.PHONY: disk
disk: boot.efi
	qemu-img create -f raw ./disk.img 200M
	mkfs.fat -F 32 ./disk.img
	mkdir -p ./mnt
	sudo mount -o loop ./disk.img ./mnt
	sudo mkdir -p ./mnt/EFI/BOOT
	sudo cp boot.efi ./mnt/EFI/BOOT/BOOTX64.EFI
	sleep 2
	sudo umount ./mnt
	rmdir mnt

.PHONY: run
run:
	qemu-system-x86_64 -m 1G -drive if=pflash,format=raw,readonly=on,file=OVMF.fd -serial tcp:127.0.0.1:4444,server,nowait -monitor stdio -vga std -drive id=ide,index=0,media=disk,format=raw,file=disk.img,if=ide

.PHONY: clean
clean:
	rm boot.efi

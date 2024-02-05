
CFLAGS = --target=i686-pc-none-elf -march=i686 -ffreestanding -Ofast -nostdlib -g -nodefaultlibs
LL = cmake-build-debug/multiboot.o cmake-build-debug/source/libk/libk.a cmake-build-debug/source/libc/libc.a cmake-build-debug/source/libMarx/libMarx.a cmake-build-debug/source/libDraco/libDraco.a cmake-build-debug/source/libDraco/Scanner/libScanner.a cmake-build-debug/source/libReflector/libReflector.a

main: multiboot link generateIso 

multiboot: source/libk/boot.s
	clang -c source/libk/boot.s $(CFLAGS) -o cmake-build-debug/multiboot.o

link: legacy/build
	ld.lld -T cmake-build-debug/linker.ld $(LL) -o myos.bin

#FIXME

generateIso: grub.cfg myos.bin
#	cp "myos.bin" "legacy/build/boot/myos.bin"
#	cp "grub.cfg" "legacy/build/boot/grub/grub.cfg"
	qemu-system-i386 -kernel myos.bin -m 256
#	grub-mkimage -O i386-pc -o myos.iso -c grub.cfg -p build/boot/
# Заменить на grub-mkrescure как только на Linux'e
#--------------------------------------Makefile-------------------------------------

CFILES = $(wildcard ./src/*.c)
OFILES = $(CFILES:./src/%.c=./build/%.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib

all: clean kernel8.img run


./build/boot.o: ./src/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./src/boot.S -o ./build/boot.o

./build/%.o: ./src/%.c 
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: ./build/boot.o $(OFILES)
	aarch64-none-elf-ld -nostdlib ./build/boot.o $(OFILES) -T ./src/link.ld -o ./build/kernel8.elf
	aarch64-none-elf-objcopy -O binary ./build/kernel8.elf  kernel8.img

clean:
	rm -f .\build\kernel8.elf .\build\*.o *.img

run: 
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial stdio
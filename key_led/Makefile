all:
	arm-linux-gcc -c -o key_led.o key_led.c
	arm-linux-gcc -c -o start.o start.S
	arm-linux-ld -Ttext 0 start.o key_led.o -o key_led.elf
	arm-linux-objcopy  -O binary -S key_led.elf ked_led.bin
	arm-linux-objdump -D key_led.elf > ked_led.dis

clean:
	rm *.o *.dis *.bin *.elf

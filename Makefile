PROJECT=genius
OBJECTS=src/main.o src/queue.o
AVRDUDE_FLAGS=-p t2313 -C ./avrdude_gpio.conf -c pi_1
FUSES=-U lfuse:w:0x7d:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
MCU=attiny2313
F_CPU=750000UL  # 6 mhz / 8

CC=avr-gcc -Wall -Wextra
CFLAGS=-Os -DF_CPU=${F_CPU} -mmcu=${MCU} -Isrc

${PROJECT}.hex: ${PROJECT}.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

${PROJECT}.elf: ${OBJECTS}
	$(CC) -mmcu=${MCU} -o $@ $^

test: src/test.c src/queue.c
	gcc -Wall -Wextra -O0 -ggdb -o $@ $^
	./$@

test-connection:
	sudo avrdude ${AVRDUDE_FLAGS}

clean:
	rm -f *.elf src/*.o test

upload: ${PROJECT}.hex
	sudo avrdude ${AVRDUDE_FLAGS} -U flash:w:$<:i

fuse:
	sudo avrdude ${AVRDUDE_FLAGS} ${FUSES}

# vim: set ts=8 sts=8 sw=8 noexpandtab:

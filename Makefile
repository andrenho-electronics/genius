PROJECT=genius
OBJECTS=src/main.o src/queue.o
AVRDUDE_FLAGS=-p t2313 -C ./avrdude_gpio.conf -c pi_1
FUSES=-U lfuse:w:0x7d:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
MCU=atmega328p

CC=avr-gcc -Wall -Wextra
CFLAGS=-Os -mmcu=${MCU} -Isrc

${PROJECT}.hex: ${PROJECT}.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

${PROJECT}.elf: ${OBJECTS}
	$(CC) -mmcu=${MCU} -o $@ $^

test-connection:
	sudo avrdude ${AVRDUDE_FLAGS}

clean:
	rm -f *.elf src/*.o

upload: ${PROJECT}.hex
	sudo avrdude ${AVRDUDE_FLAGS} -U flash:w:$<:i

fuse:
	sudo avrdude ${AVRDUDE_FLAGS} ${FUSES}

# vim: set ts=8 sts=8 sw=8 noexpandtab:

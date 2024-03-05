.PHONY: clean

bme280: bme280.c bme280.h fileio.c fileio.h
	gcc -g -Wall -Wextra -pedantic -std=c11 -D_DEFAULT_SOURCE -D_BSD_SOURCE -o bme280 bme280.c fileio.c -lwiringPi -lm

clean:
	rm bme280

CFLAGS=-Wall -D_GNU_SOURCE -lwiringPi -std=c99

therm_reader: therm.o
	gcc $(CFLAGS) -o therm_reader main.c therm.h therm.o

therm.o: therm.c therm.h
	gcc $(CFLAGS) -c therm.c therm.h

#FILE	: Makefile
#Author	: Jean "henyxia" Wasilewski	

CC=gcc
CFLAGS=-c -Wall -std=c99 -g -D_BSD_SOURCE
#LIBS=./libusb/libusb/.libs/libusb-1.0.a
LIBS=-lusb-1.0
SOURCES=main.c ui.c turret.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=turret_controller

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clear:
	rm $(OBJECTS) $(EXECUTABLE)

#gcc -c ui.c
#gcc -g -Wall -lusb-1.0 -std=c99 -L./libusb/libusb/.libs main.c

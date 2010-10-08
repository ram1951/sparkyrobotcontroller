CC = gcc -g
INCLUDE = -I/usr/local/include -Iclearsilver -I.
LIBS = -L. -lpthread -lm -lncurses -lcreateoi clearsilver/libs/libneo_utl.a
CVFLAGS = $(shell pkg-config --libs opencv)  $(shell pkg-config --cflags opencv) 

default: all


all: libcreateoi.a controld

libcreateoi.a: createoi.o
	$(AR) crv libcreateoi.a createoi.o
	ranlib libcreateoi.a

createoi.o: createoi.c
	$(CC) createoi.c $(INCLUDE) -c -o createoi.o

controld: controld.c
	$(CC) controld.c $(INCLUDE) $(LIBS) -o controld

clean:
	rm -f *.o
	rm -f controld

restart:
	killall controld
	./controld /dev/cu.KeySerial1

start:
	./controld /dev/cu.KeySerial1

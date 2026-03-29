CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2

all: vale_verde

vale_verde: main.c
	$(CC) $(CFLAGS) main.c -o vale_verde

run: vale_verde
	./vale_verde

clean:
	rm -f vale_verde

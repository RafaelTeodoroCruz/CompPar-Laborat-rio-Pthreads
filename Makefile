CC = gcc
CFLAGS = -O2 -Wall -pthread

PROGRAMAS = seq f1 f2 primos monte_carlo_pi soma_vetores

.PHONY: all clean

all: $(PROGRAMAS)

seq: contacorrente_seq.c
	$(CC) $(CFLAGS) contacorrente_seq.c -o seq

f1: contacorrente_f1.c
	$(CC) $(CFLAGS) contacorrente_f1.c -o f1

f2: contacorrente_f2.c
	$(CC) $(CFLAGS) contacorrente_f2.c -o f2

primos: primos.c
	$(CC) $(CFLAGS) primos.c -o primos

monte_carlo_pi: monte_carlo_pi.c
	$(CC) $(CFLAGS) monte_carlo_pi.c -o monte_carlo_pi

soma_vetores: soma_vetores.c
	$(CC) $(CFLAGS) soma_vetores.c -o soma_vetores

clean:
	rm -f $(PROGRAMAS)

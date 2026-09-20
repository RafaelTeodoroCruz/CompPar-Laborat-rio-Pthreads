#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_OPERACOES 50000000
#define VALOR_DEPOSITO 5.0
#define VALOR_SAQUE 2.0

double saldo = 1000.00;

void depositos(void) {
    for (long i = 0; i < NUM_OPERACOES; i++) {
        saldo += VALOR_DEPOSITO;
    }
}

void saques(void) {
    for (long i = 0; i < NUM_OPERACOES; i++) {
        saldo -= VALOR_SAQUE;
    }
}

int main(void) {
    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    depositos();
    saques();

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) +
                   (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("[Sequencial] Saldo final: %.2f | Tempo: %.4f s\n",
           saldo, tempo);

    return 0;
}

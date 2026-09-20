#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    long long pontos;
    unsigned int semente;
    long long dentro_circulo;
} DadosThread;

void* calcular_pi(void* argumento) {
    DadosThread* dados = (DadosThread*) argumento;

    dados->dentro_circulo = 0;

    for (long long i = 0; i < dados->pontos; i++) {
        double x = (double) rand_r(&dados->semente) / RAND_MAX;
        double y = (double) rand_r(&dados->semente) / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            dados->dentro_circulo++;
        }
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    long long total_pontos = atoll(argv[1]);
    int num_threads = atoi(argv[2]);

    if (total_pontos < 1 || num_threads < 1) {
        printf("Erro: N e num_threads devem ser maiores que zero.\n");
        return 1;
    }

    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    DadosThread* dados = malloc(num_threads * sizeof(DadosThread));

    if (threads == NULL || dados == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(threads);
        free(dados);
        return 1;
    }

    long long pontos_base = total_pontos / num_threads;
    long long resto = total_pontos % num_threads;

    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < num_threads; i++) {
        dados[i].pontos = pontos_base;

        if (i < resto) {
            dados[i].pontos++;
        }

        dados[i].semente = (unsigned int) time(NULL) ^ (unsigned int) (i * 1234567);
        dados[i].dentro_circulo = 0;

        if (pthread_create(&threads[i], NULL, calcular_pi, &dados[i]) != 0) {
            printf("Erro ao criar a thread %d.\n", i);
            free(threads);
            free(dados);
            return 1;
        }
    }

    long long total_dentro_circulo = 0;

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_dentro_circulo += dados[i].dentro_circulo;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double pi = 4.0 * (double) total_dentro_circulo / total_pontos;

    double tempo = (fim.tv_sec - inicio.tv_sec) +
                   (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("[Monte Carlo Pi] N: %lld | Threads: %d | Pi: %.10f | Tempo: %.4f s\n",
           total_pontos, num_threads, pi, tempo);

    free(threads);
    free(dados);

    return 0;
}

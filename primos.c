#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    long long inicio;
    long long limite;
    int id;
    int num_threads;
    long long primos_locais;
} DadosThread;

int eh_primo(long long numero) {
    if (numero < 2) {
        return 0;
    }

    if (numero == 2) {
        return 1;
    }

    if (numero % 2 == 0) {
        return 0;
    }

    for (long long divisor = 3; divisor * divisor <= numero; divisor += 2) {
        if (numero % divisor == 0) {
            return 0;
        }
    }

    return 1;
}

void* contar_primos(void* argumento) {
    DadosThread* dados = (DadosThread*) argumento;

    dados->primos_locais = 0;

    for (long long numero = 2 + dados->id;
         numero <= dados->limite;
         numero += dados->num_threads) {

        if (eh_primo(numero)) {
            dados->primos_locais++;
        }
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <K> <num_threads>\n", argv[0]);
        return 1;
    }

    long long limite = atoll(argv[1]);
    int num_threads = atoi(argv[2]);

    if (limite < 2 || num_threads < 1) {
        printf("Erro: K deve ser >= 2 e num_threads deve ser >= 1.\n");
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

    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < num_threads; i++) {
        dados[i].limite = limite;
        dados[i].id = i;
        dados[i].num_threads = num_threads;
        dados[i].primos_locais = 0;

        if (pthread_create(&threads[i], NULL, contar_primos, &dados[i]) != 0) {
            printf("Erro ao criar a thread %d.\n", i);
            free(threads);
            free(dados);
            return 1;
        }
    }

    long long total_primos = 0;

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_primos += dados[i].primos_locais;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = (fim.tv_sec - inicio.tv_sec) +
                   (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("[Primos] K: %lld | Threads: %d | Total de primos: %lld | Tempo: %.4f s\n",
           limite, num_threads, total_primos, tempo);

    free(threads);
    free(dados);

    return 0;
}

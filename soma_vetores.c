#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    const double *a;
    const double *b;
    double *c;
    size_t inicio;
    size_t fim;
} DadosThread;

void *somar_vetores(void *argumento) {
    DadosThread *dados = (DadosThread *)argumento;
    for (size_t i = dados->inicio; i < dados->fim; i++) {
        dados->c[i] = dados->a[i] + dados->b[i];
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    char *fim_n = NULL, *fim_threads = NULL;
    errno = 0;
    unsigned long long entrada_n = strtoull(argv[1], &fim_n, 10);
    if (errno || argv[1][0] == '-' || fim_n == argv[1] || *fim_n != '\0' ||
        entrada_n == 0 || entrada_n > SIZE_MAX / sizeof(double)) {
        fprintf(stderr, "Erro: N invalido ou grande demais.\n");
        return 1;
    }
    errno = 0;
    unsigned long entrada_threads = strtoul(argv[2], &fim_threads, 10);
    if (errno || argv[2][0] == '-' || fim_threads == argv[2] || *fim_threads != '\0' ||
        entrada_threads == 0 || entrada_threads > 1024) {
        fprintf(stderr, "Erro: num_threads deve estar entre 1 e 1024.\n");
        return 1;
    }

    size_t n = (size_t)entrada_n;
    size_t num_threads = (size_t)entrada_threads;
    double *a = malloc(n * sizeof(*a));
    double *b = malloc(n * sizeof(*b));
    double *c = malloc(n * sizeof(*c));
    pthread_t *threads = malloc(num_threads * sizeof(*threads));
    DadosThread *dados = malloc(num_threads * sizeof(*dados));

    if (!a || !b || !c || !threads || !dados) {
        fprintf(stderr, "Erro ao alocar memoria.\n");
        free(a); free(b); free(c); free(threads); free(dados);
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        a[i] = 1.5;
        b[i] = 2.5;
    }

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    size_t criadas = 0;
    int erro = 0;
    for (size_t i = 0; i < num_threads; i++) {
        dados[i].a = a;
        dados[i].b = b;
        dados[i].c = c;
        dados[i].inicio = (n / num_threads) * i + (n % num_threads) * i / num_threads;
        dados[i].fim = (n / num_threads) * (i + 1) + (n % num_threads) * (i + 1) / num_threads;
        /* Cada thread recebe um bloco contiguo e exclusivo do vetor. */
        int codigo = pthread_create(&threads[i], NULL, somar_vetores, &dados[i]);
        if (codigo != 0) {
            fprintf(stderr, "Erro ao criar thread %zu (codigo %d).\n", i, codigo);
            erro = 1;
            break;
        }
        criadas++;
    }

    for (size_t i = 0; i < criadas; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);

    if (!erro) {
        double tempo = (fim.tv_sec - inicio.tv_sec) +
                       (fim.tv_nsec - inicio.tv_nsec) / 1e9;
        int correto = 1;
        for (size_t i = 0; i < n; i++) {
            if (c[i] != 4.0) {
                correto = 0;
                break;
            }
        }
        printf("[Soma de Vetores] N: %zu | Threads: %zu | Resultado: %s | Tempo: %.4f s\n",
               n, num_threads, correto ? "Correto" : "Incorreto", tempo);
    }

    free(a); free(b); free(c); free(threads); free(dados);
    return erro ? 1 : 0;
}

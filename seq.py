import time


def contagem_pesada(n):
    while n > 0:
        n -= 1


if __name__ == "__main__":
    n = 100_000_000
    inicio = time.time()

    contagem_pesada(n)
    contagem_pesada(n)

    fim = time.time()
    print(f"[Python Sequencial] Tempo: {fim - inicio:.4f} segundos")

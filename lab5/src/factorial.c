#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Структура для передачи данных в потоки
typedef struct {
    int start;
    int end;
    int mod;
    long long *result;
    pthread_mutex_t *mutex;
} ThreadData;

// Функция для вычисления факториала по модулю
void *compute_factorial(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    long long local_result = 1;

    for (int i = data->start; i <= data->end; i++) {
        local_result = (local_result * i) % data->mod;
    }

    // Синхронизация доступа к общему результату
    pthread_mutex_lock(data->mutex);
    *data->result = (*data->result * local_result) % data->mod;
    pthread_mutex_unlock(data->mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int k, pnum, mod;
    int opt;

    // Чтение параметров командной строки
    while ((opt = getopt(argc, argv, "k:p:m:")) != -1) {
        switch (opt) {
            case 'k':
                k = atoi(optarg);
                break;
            case 'p':
                pnum = atoi(optarg);
                break;
            case 'm':
                mod = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -k <number> -p <threads> -m <mod>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (k <= 0 || pnum <= 0 || mod <= 0) {
        fprintf(stderr, "Invalid arguments\n");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[pnum];
    ThreadData data[pnum];
    pthread_mutex_t mutex;
    long long result = 1;

    // Инициализация мьютекса
    pthread_mutex_init(&mutex, NULL);

    // Разделение задачи на части для каждого потока
    int chunk_size = k / pnum;
    int start = 1;

    for (int i = 0; i < pnum; i++) {
        data[i].start = start;
        data[i].end = (i == pnum - 1) ? k : start + chunk_size - 1;
        data[i].mod = mod;
        data[i].result = &result;
        data[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, compute_factorial, (void *)&data[i]);

        start = data[i].end + 1;
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }

    // Уничтожение мьютекса
    pthread_mutex_destroy(&mutex);

    // Вывод результата
    printf("Factorial of %d modulo %d is: %lld\n", k, mod, result);

    return 0;
}
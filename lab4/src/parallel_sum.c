#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "utils.h"

struct SumArgs {
    int *array;
    int begin;
    int end;
};

int Sum(const struct SumArgs *args) {
    int sum = 0;
    for (int i = args->begin; i < args->end; i++) {
        sum += args->array[i];
    }
    return sum;
}

void *ThreadSum(void *args) {
    struct SumArgs *sum_args = (struct SumArgs *)args;
    return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
    uint32_t threads_num = 0;
    uint32_t array_size = 0;
    uint32_t seed = 0;

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--threads_num") == 0) {
            threads_num = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--array_size") == 0) {
            array_size = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--seed") == 0) {
            seed = atoi(argv[++i]);
        }
    }

    if (threads_num == 0 || array_size == 0 || seed == 0) {
        printf("Usage: %s --threads_num \"num\" --array_size \"num\" --seed \"num\"\n", argv[0]);
        return 1;
    }

    // Генерируем массив
    int *array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);

    // Замеряем время выполнения
    struct timeval start, end;
    gettimeofday(&start, NULL);

    pthread_t threads[threads_num];
    struct SumArgs args[threads_num];

    // Создаем потоки
    for (uint32_t i = 0; i < threads_num; i++) {
        args[i].array = array;
        args[i].begin = i * (array_size / threads_num);
        args[i].end = (i == threads_num - 1) ? array_size : (i + 1) * (array_size / threads_num);
        if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
            printf("Error: pthread_create failed!\n");
            free(array);
            return 1;
        }
    }

    int total_sum = 0;
    for (uint32_t i = 0; i < threads_num; i++) {
        int sum = 0;
        pthread_join(threads[i], (void **)&sum);
        total_sum += sum;
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0;
    elapsed += (end.tv_usec - start.tv_usec) / 1000.0;

    free(array);
    printf("Total: %d\n", total_sum);
    printf("Elapsed time: %.2f ms\n", elapsed);
    return 0;
}
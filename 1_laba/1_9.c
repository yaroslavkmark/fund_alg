#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

void find_and_swap_min_max(int *array, int size) {
    if (size <= 0) return;

    int min_index = 0, max_index = 0;
    int min_value = array[0];
    int max_value = array[0];

    for (int i = 1; i < size; i++) {
        if (array[i] < min_value) {
            min_value = array[i];
            min_index = i;
        }
        if (array[i] > max_value) {
            max_value = array[i];
            max_index = i;
        }
    }

    if (min_index != max_index) {
        int temp = array[min_index];
        array[min_index] = array[max_index];
        array[max_index] = temp;
    }
}

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int findClosest(int arr[], int size, int target) {
    int left = 0, right = size - 1;
    int closest = arr[0];

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return arr[mid];
        }

        if (abs(arr[mid] - target) < abs(closest - target)) {
            closest = arr[mid];
        }

        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return closest;
}

bool is_number(const char *str) {
    if (*str == '-') {
        str++;
    }


    bool hasDot = false;
    if (*str == '\0') return false;

    while (*str) {
        if (*str == '.') {
            if (hasDot) return false;
            hasDot = true;
        } else if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <a> <b>\n", argv[0]);
        return 1;
    }
    int a, b;
    if (is_number(argv[1]) && is_number(argv[2])){
        a = atoi(argv[1]);
        b = atoi(argv[2]);
    }
    else{
        printf("Error: is not number\n");
        return 1;
    }

    if (a > b) {
        printf("Error: a should be less than or equal to b.\n");
        return 1;
    }

    srand(time(NULL));
    int size = 10; // Фиксированный размер массива
    int array[size];

    // Заполнение массива случайными числами
    for (int i = 0; i < size; i++) {
        array[i] = rand() % (b - a + 1) + a;
    }

    // Вывод массива
    printf("Original array:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Поиск и замена минимального и максимального значений
    find_and_swap_min_max(array, size);

    // Вывод измененного массива
    printf("Modified array:\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    srand(time(NULL));

    // Генерируем размеры массивов A и B
    int sizeA = rand() % (20 - 10 + 1) + 10;
    int sizeB = rand() % (20 - 10 + 1) + 10;

    // Создаем массивы A и B
    int *A = (int *)malloc(sizeA * sizeof(int));
    int *B = (int *)malloc(sizeB * sizeof(int));
    printf("A\n");
    for (int i = 0; i < sizeA; i++) {
        A[i] = rand() % (1001 + 1000) - 1000;  // [-1000..1000]
        printf("%d ", A[i]);
    }
    printf("\nB\n");
    for (int i = 0; i < sizeB; i++) {
        B[i] = rand() % (1001 + 1000) - 1000;  // [-1000..1000]
        printf("%d ", B[i]);
    }

    // Сортируем массив B для последующего бинарного поиска
    qsort(B, sizeB, sizeof(int), compare);
    printf("\nB\n");
    for (int i = 0; i < sizeB; i++) {
        printf("%d ", B[i]);
    }
    // Создаем массив C
    int *C = (int *)malloc(sizeA * sizeof(int));
    printf("\nC\n");
    for (int i = 0; i < sizeA; i++) {
        int closest = findClosest(B, sizeB, A[i]);
        C[i] = A[i] + closest;
        printf("%d ", C[i]);
    }
    printf("\n");


    // Освобождаем память
    free(A);
    free(B);
    free(C);


    return 0;
}

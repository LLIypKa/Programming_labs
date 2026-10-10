//б. нормализация записи
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define N 16

void init_array(int*);
short* find_most_loudest_abs(int*, int*, int*);
float find_amplification(int*);
void scaling(int*, float*);
void print_arr(int*);
int cutted_count(int*);

int main(void) {
    int array[N] = {120, -3400, 8000, -12000, 500, -700, 15000, -9000,
300, -150, 6000, -3000, 900, -400, 1100, -250};
    int index, loudest, count;
    float amplification;

    print_arr(array);

    find_most_loudest_abs(array, &loudest, &index);
    printf("Самый громкий по модулю array[%d] = %hd\n", index, loudest);

    amplification = find_amplification(&loudest);
    printf("Коэффициент усиления равен %.2f\n", amplification);

    printf("Усиленный массив отсчётов\n");
    scaling(array, &amplification);
    print_arr(array);

    printf("Усиленный дважды массив отсчётов\n");
    scaling(array, &amplification);    
    count = cutted_count(array);
    print_arr(array);
    printf("В дважды усиленом массиве срезали бы %d отсчётов\n", count);

    return EXIT_SUCCESS;
}

void init_array(int *array) {
    printf("Введите %d элементов\n", N);
    for (int i = 0; i < N; i++) {
        printf("a[%d] = ", i);
        scanf("%d", &array[i]);
    }
}

short* find_most_loudest_abs(int *array, int *loudest, int *index) {
    *loudest = SHRT_MIN;
    *index = -1;

    for(int i = 0; i < N; i++) {
        if (abs(array[i]) >= *loudest) {
            *loudest = array[i];
            *index = i;
        }
    }
}

float find_amplification(int *loudest) {
    return (float)SHRT_MAX / abs(*loudest);
}

void scaling(int *array, float* amplification) {
    for (int i = 0; i < N; i++) {
        array[i] *= *amplification;
    }
}

void print_arr(int *array) {
    for (int i = 0; i < N; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int cutted_count(int *array) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (array[i] < SHRT_MIN || array[i] > SHRT_MAX) {
            count++;
        }
    }

    return count;
}
#include <stdio.h>
#include <stdlib.h>

/// @brief считает кол-во битов bit в числе number
/// @param number - число 
/// @param bit - искомый бит (0 или 1)
/// @return -1, если bit некорректный, иначе - кол-во битов bit
int bits_counter(int* number, char* bit);

int main(void) {
    int number;
    char bit = 1;

    printf("Введите целое неотрицательное число: ");
    scanf("%d", &number);
    while (number < 0) {
        printf("Вы ввели некорректное число. Введите целое неотрицательное число: ");
        scanf("%d", &number);
    }

    int count = bits_counter(&number, &bit);
    printf("Число единичных битов в числе %d: %d\n", number, count);

    return EXIT_SUCCESS;
}

int bits_counter(int* number, char* bit) {
    if (*bit != 0 && *bit != 1) {
        return -1;
    }

    int counter = 0;
    for(int i = sizeof(*number) * 8 - 1; i >= 0; i--) {
        char each_bit = (*number >> i) & 1;
        if (each_bit == *bit) {
            counter++;
        } 
    }

    return counter;
}
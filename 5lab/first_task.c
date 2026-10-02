#include <stdio.h>
#include <stdlib.h>

/// @brief считает кол-во единичных и нулевых битов в числе number
/// @param number - число 
/// @param zero_bits_count - число нулевых битов
/// @param single_bits_count - число единичных битов
void bits_counter(int* number, unsigned char* zero_bits_count, unsigned char* single_bits_count);

int main(void) {
    int number;
    unsigned char single_bits_count = 0, zero_bits_count = 0;

    printf("Введите целое неотрицательное число: ");
    scanf("%d", &number);
    while (number < 0) {
        printf("Вы ввели некорректное число. Введите целое неотрицательное число: ");
        scanf("%d", &number);
    }

    bits_counter(&number, &zero_bits_count, &single_bits_count);
    printf("В числе %d есть %d 0-битов и %d 1-битов\n", number, zero_bits_count, single_bits_count); 

    return EXIT_SUCCESS;
}

void bits_counter(int* number, unsigned char* zero_bits_count, unsigned char* single_bits_count) {
    for(int i = sizeof(*number) * 8 - 1; i >= 0; i--) {
        char each_bit = (*number >> i) & 1;
        if (each_bit == 1) {
            (*single_bits_count)++;
        } 
        else {
            (*zero_bits_count)++;
        }
    }
}
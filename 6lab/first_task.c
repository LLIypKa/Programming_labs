/*
можно упростить, если увеличить массива вдвое и сделать его типа short
тогда уйдёт несколько функций и, соотвественно, станет меньше вызовов к этим функциям и код станет проще
+ расход памяти останется тем же
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define MIN -32768
#define MAX 32767

void init_complex_array(unsigned int*);
void print_complex_array(unsigned int*);
short get_real_part(unsigned int*);
short get_imaginary_part(unsigned int*);
unsigned int get_signal_energy(unsigned int*);
void find_max(unsigned int*, short*, int*);

int main(void) {
    srand(time(NULL));

    unsigned int complex_numbers[N];
    init_complex_array(complex_numbers);
    print_complex_array(complex_numbers);
    unsigned int energy = get_signal_energy(complex_numbers);
    short max;
    int index;
    find_max(complex_numbers, &max, &index);

    printf("E = %u\n", energy);
    printf("max = %hd, index = %d\n", max, index);

    return EXIT_SUCCESS;
}

void init_complex_array(unsigned int *array) {
    int range_length = MIN * (-2);
    short real, imaginary;
    for (int i = 0; i < N; i++) {
        real = rand() % range_length + MIN;
        imaginary = rand() % range_length + MIN;
        //здесь (unsigned) чтобы не нарваться на UB при сдвиге отрицательного числа
        array[i] = ((unsigned int)imaginary << 16) | ((unsigned short) real);
    }
}

void print_complex_array(unsigned int *array) {
    for (int i = 0; i < N; i++) {
        printf("array[%d] = %hd + i*(%hd)\n", i, get_real_part(&array[i]), get_imaginary_part(&array[i]));
    }
}

short get_real_part(unsigned int *number) {
    return *number & 0xFFFF;
}

short get_imaginary_part(unsigned int *number) {
    return (*number >> 16) & 0xFFFF;
}

unsigned int get_signal_energy(unsigned int *array)  {
    unsigned int E = 0;
    for (int i = 0; i < N; i++) {
        short real = get_real_part(&array[i]);
        short imaginary = get_imaginary_part(&array[i]);

        E += (unsigned int)(real * real) + (unsigned int)(imaginary * imaginary); 
    }

    return E;
}

void find_max(unsigned int *array, short *max, int *index) {
    *max = MIN;
    *index = -1;

    for (int i = 0; i < N; i++) {
        short real = get_real_part(&array[i]);
        if (real >= *max) {
            *max = real;
            *index = i;
        }
    }
}
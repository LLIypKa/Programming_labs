#include <stdio.h>
#include <stdlib.h>

int check_is_numbrer_simple(int*);

int main(void) {
    int e, n;

    printf("Введите число n (модуль шифра) и число e (открытй ключ) через пробел\n");
    scanf("%d %d", &e, &n);
    
    for (int divider = 2; divider < (n / 2); divider++) {
        if (n % divider == 0) {
            if (check_is_number_simple(divider)) {
                
            }  
        } 
    }

    return EXIT_SUCCESS;
}

int check_is_number_simple(int* number) {
    for (int divider = 2; divider < number; divider++) {
        if (*number % divider == 0) {
            return 0;
        } 
    }

    return 1;
}
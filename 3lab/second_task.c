#include <stdio.h>
#include <stdlib.h>

int check_is_number_simple(int*);
int decode(int*, int*, int*);
int cycle(int* cipher, int* d, int* n);

int main(void) {
    int e, n, condititon, code;
    int p, q;
    int phi;
    int d;
    char symb;

    printf("Введите число n (модуль шифра) и число e (открытй ключ) через пробел, а также зашифрованное сообщение\n");
    scanf("%d %d %d", &n, &e, &code);
    
    condititon = n / 2;
    for (int divider = 1; divider <= condititon; divider += 2) {
        if (n % divider == 0) {
            int divider2 = n / divider;
            int is_divider1_simple = check_is_number_simple(&divider);
            int is_divider2_simple = check_is_number_simple(&divider2);
            if (is_divider1_simple && is_divider2_simple) {
                p = divider;
                q = divider2;
                break;
            }
        } 
    }

    phi = (p - 1) * (q - 1);
    d = phi / e + 1;

    int decoded = decode(&d, &n, &code);

    printf("%c\n", decoded);

    return EXIT_SUCCESS;
}

int check_is_number_simple(int* number) {
    for (int divider = 2; divider < *number; divider++) {
        if (*number % divider == 0) {
            return 0;
        } 
    }

    return 1;
}

int decode(int* d, int* n, int* code) {
    char decoded_symb = cycle(code, d, n);
}

int cycle(int* cipher, int* d, int* n) {
    int result = 1;
    *cipher %= *n;

    while (*d > 0) {
        if (*d == 1) {
            result = (result * *cipher) % *n;
        }

        *cipher = (*cipher * *cipher) % *n;
        *d = *d >> 1;
    }
}
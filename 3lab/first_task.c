#include <stdio.h>
#include <stdlib.h>

void task_15();
void task_16();
void task_17();
void task_1();
void task_2();

int main(void) {
    task_15();
        
    return EXIT_SUCCESS;
}

/// @brief проверка, простое ли число или нет
void task_15() {
    int s, n;
    int a;

    printf("Введите число a: ");
    scanf("%d", &a);

    for (s = 0, n = 2; n < a; n++) {
        printf("a = %3d, n = %3d\n", a, n);
        if (a % n == 0) {
            s++;
            printf("%3d mod %3d = %3d, s = %d\n", a, n, a % n, s);
        }
    }

    if (s == 0) {
        puts("Good");
    }
}

/// @brief и снова проверка, простое ли число или нет, только теперь на первом же делителе останавливается цикл
void task_16() {
    int i, a;

    printf("Введите число a: ");
    scanf("%d", &a);

    for (i = 2; i < a; i++) {
        printf("a = %3d, i = %3d\n", a, i);
        if (a % i == 0) {
            printf("a = %3d mod i = %3d, break\n", a, i);
            break;
        }
    }

    if (i == a) {
        puts("Good");
    }
}
/// @brief и снова проверка, простое ли число или нет, только теперь для нечётных
void task_17() {
    int b, n = 2, flag = 0;

    printf("Введите число b: ");
    scanf("%d", &b);

    while(b % n != 0){
        printf("before b = %3d, n = %3d\n", b, n);
        n++;
        printf("after b = %4d, n = %3d\n", b, n);
        if (n == b) {
            puts("n = b, break");
            flag = 1;
            break;
        }
    }
}

//НОД
void task_1() {
    int n1, n2, i;
    
    printf("Введите через пробел числа n1, n2, такие что n1 < n2\n");
    scanf("%d %d", &n1, &n2);

    for (i = n1; !(n1 % i == 0 && n2 % i ==0); i--) {
        printf("1. n1 = %3d n2 = %3d\n", n1, n2);
        printf("2. !(n1(%d)) mod i(%d) == 0 && n2(%d) mod i(%d) ==0)\n", n1, i, n2, i);
    }

    printf("i = %d\n", i);
}

/// @brief НОК
void task_2() {
    int n, a, b;

    printf("Введите через пробел числа a, b, такие что a < b\n");
    scanf("%d %d", &a, &b);

    for (n = a; n % a != 0 || n % b != 0; n++) {
        printf("1. n = %3d a = %3d b = %3d\n", n, a, b);
        printf("n(%d) mod a(%d) != 0 || n(%d) mod b(%d) != 0", n, a, n, b);
    }

    printf("n = %d\n", n);
}
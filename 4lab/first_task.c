#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int N;
    
    printf("Введите N: \n");
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        for (int j = 1; j <= N; j++) {
            printf("%7d", j + N * i);
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}
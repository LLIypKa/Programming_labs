#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char letter = 65;
    int matrix_size;
    
    printf("Введите размерность матрицы: ");
    scanf("%d", &matrix_size);

    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            if (j >= i) {
                printf("%c", letter);
            }
            else {
                printf(" ");
            }
        }
        printf("\n");
        letter += 1;
    }

    return EXIT_SUCCESS;
}
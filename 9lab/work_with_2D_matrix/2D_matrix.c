#include <stdio.h>
#include "2D_matrix.h"

#define FORMAT(x) _Generic((x), \
    int: "%d",                  \
    float: "%.2f",              \
    double: "%.2f"              \
)

/// @brief печатает двумерную матрицу
/// @param matrix матрица с неизвестным типом
/// @param n количество строк в матрице
/// @param m количество столбцов в матрице
/// @param type тип элементов
/// @returns 0 - успешно выведено, -1 - возникли ошибки
int print_matrix(const void* matrix, size_t n, size_t m, enum types type) {
    if (!matrix) return -1;

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            size_t k = i * m + j;          
            switch (type) {
                case MATRIX_INT:    
                    printf(" %d ", ((int*)matrix)[k]); 
                    break;
                case MATRIX_FLOAT:  
                    printf(" %.12f ", ((float*)matrix)[k]); 
                    break;
                case MATRIX_DOUBLE: 
                    printf(" %.12f ", ((double *)matrix)[k]); 
                    break;
                case MATRIX_CHAR:
                    printf(" %c ", ((char*)matrix)[k]);
                    break;
                default: 
                    return -1;
            }
        }
        putchar('\n');
    }
    return 0;
}
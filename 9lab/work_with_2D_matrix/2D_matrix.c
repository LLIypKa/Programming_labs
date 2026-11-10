#include <stdio.h>
#include "2D_matrix.h"

#define FORMAT(x) _Generic((x), \
    int: "%d",                  \
    float: "%.2f",              \
    double: "%.2f"              \
)


void print_matrix(void** matrix, size_t *n, size_t *m) {
    for(size_t i = 0; i < *n; i++) {
        for(size_t j = 0; j < *m; j++) {
            
        }
    }
}

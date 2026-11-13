#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

enum types { MATRIX_INT, MATRIX_FLOAT, MATRIX_DOUBLE, MATRIX_CHAR };
int print_matrix(const void *matrix, size_t n, size_t m, enum types type);

#endif /*MATRIX_H*/
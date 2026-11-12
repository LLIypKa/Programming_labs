#ifndef MATRIX_H
#define MATRIX_H

enum types { INT, FLOAT, DOUBLE };

int print_matrix(void *matrix, size_t n, size_t m, enum types type);

#endif /*MATRIX_H*/
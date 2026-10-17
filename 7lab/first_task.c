#include <stdio.h>
#include <stdlib.h>

void init_matrix(double**, double*, int*);
double find_determinant(double**, int*);
double find_minor_determinant(double**, int*, int, int*);
int get_multiplier(int, int);
void find_unknowns(double**, double*, double*, int*, double*);

int main(void) {
    int matrix_size;
    double determinant = 0;
    
    printf("Введите размерность матрицы NxN: ");
    scanf("%d", &matrix_size);
    //инициализация памяти для матрицы
    double **matrix = (double**)malloc(sizeof(double*) * matrix_size);
    double *free_terms_vector = (double*)malloc(sizeof(double) * matrix_size);
    for (int i = 0; i < matrix_size; i++) {
        matrix[i] = (double*)malloc(sizeof(double) * matrix_size);
    }
    init_matrix(matrix, free_terms_vector, &matrix_size);

    determinant = find_determinant(matrix, &matrix_size);
    if (determinant == 0) {
        printf("Решений или нет, или их бесконечно много");
        return EXIT_SUCCESS;
    }
    else {
        printf("det A = %.2lf\n", determinant);
    }

    double *unknowns_vector = (double*)malloc(sizeof(double) * matrix_size);
    find_unknowns(matrix, free_terms_vector, unknowns_vector, &matrix_size, &determinant);

    for(int i = 0; i < matrix_size; i++) {
        printf("x%d = %.2lf\n", i, unknowns_vector[i]);
    }

    for(int i = 0; i < matrix_size; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(free_terms_vector);
    free(unknowns_vector);
    return EXIT_SUCCESS;
}

void init_matrix(double **matrix, double *free_terms, int* matrix_size) {
    for (int i = 0; i < *matrix_size; i++) {
        for (int j = 0; j < *matrix_size; j++) {
            printf("Введите [%d][%d] элемент матрицы: ", i + 1, j + 1);
            scanf("%lf", &matrix[i][j]);
        }
    }

    printf("Введите столбец свободных членов\n");
    for (int i = 0; i < *matrix_size; i++) {
        printf("B[%d] = ", i);
        scanf("%lf", &free_terms[i]);
    }
}

double find_determinant(double **matrix, int* matrix_size) {
    double determinant = 0;
    int *used_columns = (int*)calloc(*matrix_size, sizeof(int));

    for (int i = 0; i < *matrix_size; i++) {
        used_columns[i] = 1;
        determinant += get_multiplier(0, i) * matrix[0][i] * find_minor_determinant(matrix, matrix_size, 0, used_columns);
        used_columns[i] = 0;
    }

    free(used_columns);
    return determinant;
}

/// @brief рекурсивно ищет определитель минора разложением по строке
/// @param matrix сама матрица коэффициентов
/// @param matrix_size размерность матрицы
/// @param current_row_index строка ведущего элемента
/// @param used_columns массив исключенных столбцов
/// @return возвращает определитель минора
double find_minor_determinant(double **matrix, int* matrix_size, int current_row_index, int *used_columns) {
    double determinant = 0;
    int row = current_row_index + 1;

    if (row == *matrix_size) {
        return 1.0;
    }

    int position_in_minor = 0;
    for (int i = 0; i < *matrix_size; i++) {
        if (!used_columns[i]) {
            used_columns[i] = 1;
            determinant += get_multiplier(0, position_in_minor) * matrix[row][i] * find_minor_determinant(matrix, matrix_size, row, used_columns);
            position_in_minor++;
            used_columns[i] = 0;
        }
    }

    return determinant;
}

int get_multiplier(int row, int column) {
    return (row + column) % 2 == 0 ? 1 : -1;
}

void find_unknowns(double **matrix, double *free_terms, double *unknowns, int *matrix_size, double *determinant) {
    double *column_backup = (double*)malloc(sizeof(double) * *matrix_size);
    for(int i = 0; i < *matrix_size; i++) {
        for (int j = 0; j < *matrix_size; j++) {
            column_backup[j] = matrix[j][i];
            matrix[j][i] = free_terms[j];
        }

        double determinant_i = find_determinant(matrix, matrix_size);
        unknowns[i] = determinant_i / *determinant;
        for (int j = 0; j < *matrix_size; j++) {
            matrix[j][i] = column_backup[j];
        }
    }

    free(column_backup);
}
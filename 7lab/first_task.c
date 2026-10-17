#include <stdio.h>
#include <stdlib.h>
//todo: сделать перестроение минора или переделать ограничение по столбцам (например, массив использованных столбцов)
void init_matrix(double**, double*, int*);
double find_determinant(double**, int*);
double find_minor(double**, int*, int, int);
int get_multiplier(int, int);

int main(void) {
    int matrix_size;
    double determinant = 0;
    
    printf("Введите размерность матрицы NxN: ");
    scanf("%d", &matrix_size);
    //инициализация памяти для матрицы
    double **matrix = (double**)malloc(sizeof(double) * matrix_size);
    double *free_terms_vector = (double*)malloc(sizeof(double) * matrix_size);
    for (int i = 0; i < matrix_size; i++) {
        matrix[i] = (double*)malloc(sizeof(double) * matrix_size);
    }
    init_matrix(matrix, free_terms_vector, &matrix_size);

    determinant = find_determinant(matrix, &matrix_size);
    if (determinant == 0) {
        printf("Решений или нет, или их бесконечно много");
    }
    else {
        printf("det A = %.2lf\n", determinant);
    }

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
    for (int i = 0; i < *matrix_size; i++) {
        determinant += get_multiplier(0, i) * matrix[0][i] * find_minor(matrix, matrix_size, 0, i);
    }

    return determinant;
}

/// @brief рекурсивно ищет определитель минора разложением по строке
/// @param matrix сама матрица коэффициентов
/// @param matrix_size размерность матрицы
/// @param current_row_index строка ведущего элемента
/// @param current_column_index столбец ведущего элемента
/// @return возвращает определитель минора, помноженный на сам элемент
double find_minor(double **matrix, int* matrix_size, int current_row_index, int current_column_index) {
    double determinant = 0;
    int row = current_row_index + 1;

    //*matrix_size - 1 - т.к. сам по себе индекс matrix_size - это выход за границы массива
    if ((*matrix_size - 1) - row == 0) {
        return matrix[row][row];
    }

    //current_row_index есть ограничитель для просмотра столбцов (грубо говоря, количество усечённых столбцов - 1)
    //нужен, т.к. мы не пересобираем матрицу, а ищем определители миноров по той же матрице
    int pos_counter = 0;
    for (int i = current_row_index; i < *matrix_size; i++) {
        if (i != current_column_index) {
            determinant += get_multiplier(pos_counter, i) * matrix[row][i] * find_minor(matrix, matrix_size, row, i);
            pos_counter++;
        }
    }

    return determinant;
}
/*
double find_minor(double **matrix, int *matrix_size, ) {
    double minor_determinant = 0;

}
*/
int get_multiplier(int row, int column) {
    return (row + column) % 2 == 0 ? 1 : -1;
}
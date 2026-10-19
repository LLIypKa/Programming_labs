#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#define SMALL_SQUARE_SIZE 3

void init_matrix(int**, int*);
bool check_rows(int**, int*, bool*);
bool check_columns(int**, int*, bool*);
bool check_squares(int**, int*, bool*);
bool check_for_sudoku(int**, int*);
void clear_used(bool*, int*);

int main(void) {
    int matrix_size;

    printf("Введите размерность матрицы судоку n x n: ");
    scanf("%d", &matrix_size);

    int **matrix = (int**)malloc(sizeof(int*) * matrix_size); 
    init_matrix(matrix, &matrix_size);
    
    if (check_for_sudoku(matrix, &matrix_size)) {
        printf("Матрица является решением судоку\n");
    }
    else {
        printf("Матрица не является решением судоку\n");
    }

    return 0;
}

void init_matrix(int **matrix, int *matrix_size) {
    for(int i = 0; i < *matrix_size; i++) {
        matrix[i] = (int*)malloc(sizeof(int) * *matrix_size);
        for(int j = 0; j < *matrix_size; j++) {
            printf("Введите [%d][%d] элемент матрицы: ", i, j); 
            scanf("%d", &matrix[i][j]); 
        }
    }
}

bool check_for_sudoku(int **matrix, int *matrix_size) {
    bool *used_numbers = (bool*)calloc(*matrix_size, sizeof(bool));
    if (check_rows(matrix, matrix_size, used_numbers)
        && check_columns(matrix, matrix_size, used_numbers)
        && check_squares(matrix, matrix_size, used_numbers)) {
        
        free(used_numbers);
        return true;
    }

    free(used_numbers);
    return false;
}

//check_rows можно объединить с check_columns
bool check_rows(int **matrix, int *matrix_size, bool *used_numbers) {
    bool is_valid = true;

    for(int i = 0; i < *matrix_size; i++) {        
        for(int j = 0; j < *matrix_size; j++) {
            if (used_numbers[matrix[i][j] - 1]) {
                is_valid = false;
                break;
            }

            used_numbers[matrix[i][j] - 1] = true;
        }
    }

    clear_used(used_numbers, matrix_size);
    return is_valid;
}

bool check_columns(int **matrix, int *matrix_size, bool *used_numbers) {
    bool is_valid = true; 

    for(int i = 0; i < *matrix_size; i++) {
        for(int j = 0; j < *matrix_size; j++) {
            if (used_numbers[matrix[j][i] - 1]) {
                is_valid = false;
                break;
            }

            used_numbers[matrix[j][i] - 1] = true; 
        }
    }

    clear_used(used_numbers, matrix_size);
    return is_valid;
}

bool check_squares(int **matrix, int *matrix_size, bool *used_numbers) {

    return true; 
}

void clear_used(bool *used_numbers, int *matrix_size) {
    for (int i = 0; i < *matrix_size; i++) {
        used_numbers[i] = false;
    }
}
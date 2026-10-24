//АЛАРМ!!!!!! В рабочем коде нужны были проверки на NULL для выделенной памяти, а также проверки на
// кооректность чтения + проверки на корректность значения в матрице судоку (value < 1 || value > matrix_size)
// также, был бы это прод, было бы неплохо прикрутить освобождение памяти для всех динамических массивов
// прикурить проверку на корректность small_square_size (т.к. он может быть нулевым или некорректным по отношению к matrix_size)
// проверяет только квадратные судоку ()
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int** init_matrix(int*, int*, FILE*);
bool check_rows(int**, int*, bool*);
bool check_columns(int**, int*, bool*);
bool check_squares(int**, int*, int*, bool*);
bool check_for_sudoku(int**, int*, int*);
void clear_used(bool*, int*);

int main(void) {
    FILE* input = fopen("input.txt", "r");

    if (input == NULL) {
        printf("Ошибка чтения файла\n");
        return EXIT_FAILURE;
    }

    int matrix_size, small_square_size;
    int **matrix = init_matrix(&matrix_size, &small_square_size, input);
    
    if (check_for_sudoku(matrix, &matrix_size, &small_square_size)) {
        printf("Матрица является решением судоку\n");
    }
    else {
        printf("Матрица не является решением судоку\n");
    }

    fclose(input);
    return 0;
}

int** init_matrix(int *matrix_size, int *small_square_size, FILE* input) {
    fscanf(input, "%d", matrix_size);
    fscanf(input, "%d", small_square_size);

    int **matrix = (int**)malloc(sizeof(int*) * *matrix_size); 
    for(int i = 0; i < *matrix_size; i++) {
        matrix[i] = (int*)malloc(sizeof(int) * *matrix_size);
        for(int j = 0; j < *matrix_size; j++) {
            fscanf(input, "%d", &matrix[i][j]); 
        }
    }

    return matrix;
}

bool check_for_sudoku(int **matrix, int *matrix_size, int *small_square_size) {
    bool *used_numbers = (bool*)calloc(*matrix_size, sizeof(bool));
    bool is_valid = check_rows(matrix, matrix_size, used_numbers)
        && check_columns(matrix, matrix_size, used_numbers)
        && check_squares(matrix, matrix_size, small_square_size, used_numbers);
    
    free(used_numbers);
    return is_valid;
}

//check_rows можно объединить с check_columns
bool check_rows(int **matrix, int *matrix_size, bool *used_numbers) {
    bool is_valid = true;

    for(int i = 0; i < *matrix_size && is_valid; i++) {    
        clear_used(used_numbers, matrix_size);    
        for(int j = 0; j < *matrix_size; j++) {
            if (used_numbers[matrix[i][j] - 1]) {
                is_valid = false;
                break;
            }

            used_numbers[matrix[i][j] - 1] = true;
        }
    }

    return is_valid;
}

bool check_columns(int **matrix, int *matrix_size, bool *used_numbers) {
    bool is_valid = true; 

    for(int i = 0; i < *matrix_size && is_valid; i++) {
        clear_used(used_numbers, matrix_size);
        for(int j = 0; j < *matrix_size; j++) {
            if (used_numbers[matrix[j][i] - 1]) {
                is_valid = false;
                break;
            }

            used_numbers[matrix[j][i] - 1] = true; 
        }
    }

    return is_valid;
}

bool check_squares(int **matrix, int *matrix_size, int *small_square_size, bool *used_numbers) {
    bool is_valid = true; 
    int squares_per_side = *matrix_size / *small_square_size;

    for (int i = 0; i < *matrix_size && is_valid; i++) {
        clear_used(used_numbers, matrix_size);
        int start_row = (i / squares_per_side) * *small_square_size;
        int start_col = (i % squares_per_side) * *small_square_size;

        for (int j = start_row; j < start_row + *small_square_size && is_valid; j++) {
            for (int z = start_col; z < start_col + *small_square_size; z++) {
                if (used_numbers[matrix[j][z] - 1]) {
                    is_valid = false;
                    break;
                }

                used_numbers[matrix[j][z] - 1] = true; 
            }
        }
    }

    return is_valid; 
}

void clear_used(bool *used_numbers, int *matrix_size) {
    for (int i = 0; i < *matrix_size; i++) {
        used_numbers[i] = false;
    }
}
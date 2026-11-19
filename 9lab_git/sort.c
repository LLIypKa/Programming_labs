/// @brief реализация пузырьковой сортировки для массива
/// @param array указатель на массив
/// @param n размерность массива
void bubble_cort(int *array, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp;
                temp = array[j + 1];
                array[j + 1] = array[j];
                array[j] = temp;
            }            
        }
    }
}
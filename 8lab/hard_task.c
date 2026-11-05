#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//По условию задачи проверяем сдвиг максимум на 26 единиц
#define MAX_OFFSET 26
#define A_CODE 65
#define Z_CODE 90
#define a_CODE 97
#define z_CODE 122

int find_offset_and_decrypt(char*, char*, int*);
char* decrypt_with_any_offset(char*, int, int*);
int check_symbols(char*, int*);

int main(void) {
    int length = 0, letter, offset;
    char *encrypted_str = NULL, *decrypted_str = NULL;

    while ((letter = getchar()) != EOF) {
        length++;
        char *pointer = (char*)realloc(encrypted_str, length);

        if (pointer == NULL) {
            printf("Ошибка чтения/выделения памяти\n");
            free(encrypted_str);
            return EXIT_FAILURE;
        }
        
        encrypted_str = pointer;
        encrypted_str[length - 1] = (char)letter;
    }

    decrypted_str = (char*)malloc(sizeof(char) * (length + 1));
    if (decrypted_str == NULL) {
        free(decrypted_str);
        printf("Ошибка чтения/выделения памяти\n");
        return EXIT_FAILURE;
    }

    offset = find_offset_and_decrypt(encrypted_str, decrypted_str, &length);
    if (offset == INT_MIN) {
        printf("Ошибка расшифровки\n");
        free(encrypted_str);
        return EXIT_FAILURE;
    }

    printf("offset = %d\n%s\n", offset, decrypted_str);
    
    free(encrypted_str);
    return EXIT_SUCCESS;
}

/// @brief запускает процесс расшифровки, 
/// возвращает использованный сдвиг и пишет по указанному адресу расшифрованную строку
/// @param encrypted зашифрованная строка
/// @param decrypted адрес для расшифрованной строки
/// @param length длина строки
/// @return сдвиг
int find_offset_and_decrypt(char *encrypted, char *decrypted, int *length) {
    //*2 т.к. нужно проверить еще отрицательные сдвиги до 26
    char **decrypt_array = (char**)malloc(sizeof(char*) * (MAX_OFFSET));
    if (decrypt_array == NULL) {
        return INT_MIN;
    }

    int max_offset = INT_MIN;
    int max_symbols_count = INT_MIN;
    for (int i = 0; i < MAX_OFFSET; i++) {
        decrypt_array[i] = decrypt_with_any_offset(encrypted, i, length);
        if (decrypt_array[i] == NULL) {
            return INT_MIN;
        }

        int main_letters_count = check_symbols(decrypt_array[i], length);
        if (main_letters_count >= max_symbols_count) {
            max_symbols_count = main_letters_count;
            max_offset = i;
            memcpy(decrypted, decrypt_array[i], (size_t)*length + 1); 
        }
    }

    return max_offset;
}

/// @brief возвращает строку, расшифрованную с определенным сдвигом
/// @param encrypted зашифрованная строка
/// @param offset сдвиг, с которым расшифровываем
/// @param length длина строки
/// @return расшифрованная строка 
char* decrypt_with_any_offset(char *encrypted, int offset, int *length) {
    char *decrypted = (char*)malloc(sizeof(char) * (*length + 1));
    if (decrypted == NULL) {
        return decrypted;
    }

    for (int i = 0; i < *length; i++) {
        if (encrypted[i] >= A_CODE && encrypted[i] <= Z_CODE) {
            char shift = (char)(encrypted[i] - A_CODE - offset) % MAX_OFFSET; 
            shift = shift < 0 ? shift += MAX_OFFSET : shift;
            decrypted[i] = (char)(A_CODE + shift);
        }
        else if (encrypted[i] >= a_CODE && encrypted[i] <= z_CODE) {
            char shift = (char)(encrypted[i] - a_CODE - offset) % MAX_OFFSET; 
            shift = shift < 0 ? shift += MAX_OFFSET : shift;
            decrypted[i] = (char)(a_CODE + shift);
        }
        else {
            decrypted[i] = encrypted[i];
        }
    }
    decrypted[*length] = '\0';

    return decrypted;
}

int check_symbols(char *decrypted, int *length) {
    int counter = 0;
    for (int i = 0; i < *length; i++) {
        if (decrypted[i] == 'e' || decrypted[i] == 't'
                || decrypted[i] == 'E' || decrypted[i] == 'T') {
            counter++;
        }
    }

    return counter;
}
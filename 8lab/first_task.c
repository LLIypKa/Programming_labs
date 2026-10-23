#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char** split(char*, char, int, int*);
void revert_output(char**, int*);

//6
int main(void) {
    char text[] = "one two three four\0";
    int words_count = 0;
    //strlen + 1 т.к. strlen возвращает длину строки, а не размер (т.е. не учитывает нуль-терминатор)
    char **splitted_text = split(text, ' ', (int) strlen(text) + 1, &words_count);
    revert_output(splitted_text, &words_count);

    return EXIT_SUCCESS;
}

char** split(char *str, char delimiter, int str_len, int *words_count) {
    int word_len = 0;
    for (int i = 0; i < str_len; i++) {
        //без скобочек будет равносильно *(words_count + 1)
        if (str[i] == ' ' || str[i] == '\0') (*words_count)++;
    }

    int each_word = 0;
    char **splitted = (char**)malloc(sizeof(char*) * *words_count);
    for (int i = 0; i < str_len; i++) {
        if (str[i] != delimiter && str[i] != '\0') word_len++;
        else {
            splitted[each_word] = (char*)malloc(sizeof(char) * (word_len + 1));
            char* word = splitted[each_word];
            for (int j = 0; j < word_len; j++) {
                int letter_index = i - word_len + j;
                word[j] = str[letter_index]; 
            }
            word[word_len] = '\0';
            word_len = 0;
            each_word++;
        }
    }

    return splitted;
}

void revert_output(char **splitted, int *words_count) {
    for (int i = *words_count - 1; i >= 0; i--)
        printf("%s ", splitted[i]);
    printf("\n");
}
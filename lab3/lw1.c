#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const int DIFF_IN_LETTER_CASE = 32;

char *ReadLine(int *isEOF) {
    char *s = NULL;
    size_t len = 0;
    int chCode;
    while ((chCode = getchar()) != '\n' && chCode != EOF) {
        char *tmp = realloc(s, len + 1);
        if (tmp == NULL) {
            goto error;
        }
        s = tmp;
        s[len++] = (char)chCode;
    }
    if (chCode == EOF) {
        *isEOF = 1;
    }
    char *tmp = realloc(s, len + 1);
    if (tmp == NULL) {
        goto error;
    }
    s = tmp;
    s[len] = '\0';
    return s;
error:
    free(s);
    return NULL;
}

int isPunctMark(char ch) { //TODO Сделать проверку на букву
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) {
        return 0;
    }
    return 1;
}

int areDifferentLetter(int ch1, int ch2) {
    if (ch1 + DIFF_IN_LETTER_CASE == ch2 || ch1 - DIFF_IN_LETTER_CASE == ch2) {
        return 0;
    }
    return 1;
}

int isPalindromeLine(char *line) {
    int len = strlen(line);
    for (int i = 0, j = len - 1; i <= j ; i++, j--) { //TODO Сходятся и идут дальше и ф-ия слишком сложна
        while (isPunctMark(line[i]) && i + 1 < len) {
            i++;
        }
        while (isPunctMark(line[j]) && j - 1 >= 0) {
            j--;
        }
        if (line[i] != line[j] && areDifferentLetter(line[i], line[j])) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int isEOF = 0;
    while (isEOF == 0) {
        char *line = ReadLine(&isEOF);
        if (line == NULL) {
            return ENOMEM;
        }
        if (isPalindromeLine(line) == 1) {
            puts(line);
        }
        free(line);
    }
    return EXIT_SUCCESS;
}
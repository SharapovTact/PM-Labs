#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
const int DIFF_IN_LETTER_CASE = 32;
char *readLine(int *isEOF) {
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

int isPunctMark(char ch) {
    if (ch >= ' ' && ch <= '.') {
        return 1;
    }
    return 0;
}

int areDifferentLetterCase(int ch1, int ch2) {
    if (ch1 + DIFF_IN_LETTER_CASE == ch2 || ch1 - DIFF_IN_LETTER_CASE == ch2) {
        return 0;
    }
    return 1;
}

int isPalindromeLine(char *line) {
    int len = strlen(line);
    for (int i = 0, j = len - 1; i < len && j >= 0; i++, j--) {
        while (isPunctMark(line[i]) == 1 && i + 1 < len) {
            i++;
        }
        while (isPunctMark(line[j]) == 1 && j - 1 >= 0) {
            j--;
        }
        if (line[i] != line[j] && areDifferentLetterCase(line[i], line[j]) == 1) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int isEOF = 0;
    while (isEOF == 0) {
        char *line = readLine(&isEOF);
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
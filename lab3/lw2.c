#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
const int DEFAULT_STR_LEN = 4;

char *readLine(int *isEOF) {
    char *s = NULL;
    size_t len = 0;
    int chCode;
    int size = DEFAULT_STR_LEN;
    char *tmp = malloc(size);
    while ((chCode = getchar()) != '\n' && chCode != EOF) {
        if (len == size) {
            size *= 2;
            tmp = realloc(s, size);
        }
        if (tmp == NULL) {
            goto error;
        }
        s = tmp;
        s[len++] = (char)chCode;
    }
    if (chCode == EOF) {
        *isEOF = 1;
    }
    tmp = realloc(s, len + 1);
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

int fillLinesArray(int countOfLines, char **linesArray) {
    int isEOF = 0;
    for (int i = 0; i < countOfLines; i++) {
        char *line = readLine(&isEOF);
        if (line == NULL) {
            return ENOMEM;
        }
        linesArray[i] = line;
    }
    return 0;
}

int compareStrings(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

int printTop(char **linesArray, int countOfLines) {
    qsort(&*linesArray, countOfLines, sizeof(const char*), compareStrings);
    for (int i = 0; i < 3 && i < countOfLines; i++) {
        puts(linesArray[i]);
    }
    return 0;
}

void freeArray(char **linesArray, int countOfLines) {
    for (int i = countOfLines - 1; i >= 0; i--) {
        free(linesArray[i]);
    }
    free(linesArray);
}

int main(void) {
    int countOfLines;
    printf("Введите число строк: ");
    if (scanf("%d", &countOfLines) == 0) {
        puts("Ошибка ввода!");
        goto error_input;
    }
    if (countOfLines <= 0) {
        goto error_input;
    }
    getchar();

    char **linesArray = malloc(countOfLines * sizeof(char *));
    if (linesArray == NULL) {
        goto error_memory;
    }
    if (fillLinesArray(countOfLines, &*linesArray) != 0) {
        free(linesArray);
        goto error_memory;
    }

    printTop(&*linesArray, countOfLines);

    freeArray(&*linesArray, countOfLines);
    return EXIT_SUCCESS;

error_input:
    return EXIT_FAILURE;
error_memory:
    freeArray(&*linesArray, countOfLines);
    return ENOMEM;
}
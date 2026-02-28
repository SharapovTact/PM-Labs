#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

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

int myStrCmpLowerCase(char *line1, char *line2) {
    int len1 = strlen(line1);
    int len2 = strlen(line2);
    if (len1 < len2) {
        return -1;
    }
    if (len1 > len2) {
        return 1;
    }
    for (int i = 0; i < len1; i++) {
        if (tolower((int)line1) < tolower((int)line2)) {
            return -1;
        }
        if (tolower((int)line1) > tolower((int)line2)) {
            return 1;
        }
    }
    return 0;
}

int bubbleSortLinesArray(char **linesArray, int countOfLines) {
    char *tmp;
    for (int i = 0; i < countOfLines - 1; i++) {
        for (int j = i + 1; j < countOfLines; j++) {
            if (myStrCmpLowerCase(linesArray[i], linesArray[j]) < 0) {
                tmp = linesArray[i];
                linesArray[i] = linesArray[j];
                linesArray[j] = tmp;
            }
        }
    }
    return 0;
}

int printTop(char **linesArray, int countOfLines) {
    bubbleSortLinesArray(&*linesArray, countOfLines);
    for (int i = 0; i < 3 && i < countOfLines; i++) {
        puts(linesArray[i]);
    }
    return 0;
}

void freeArray(char **linesArray, int countOfLines) {
    for (int i = 0; i < countOfLines; i++) {
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
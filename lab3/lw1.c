#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

int isPalindromeLine(char *line) {
    int len = strlen(line);
    for (int i = 0, j = len - 1; i < len && j >= 0; i++, j--) {
        if (line[i] != line[j]) {
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
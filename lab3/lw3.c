#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

const int DEFAULT_STR_LEN = 4;
const int DEFAILT_ARRAY_SIZE = 4;

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

void freeArray(char **linesArray, const int countOfLines) {
    for (int i = countOfLines - 1; i >= 0; i--) {
        free(linesArray[i]);
    }
    free(linesArray);
}

char **fillLinesArray(int *countOfLines) {
    int isEOF = 0;
    *countOfLines = 0;
    int size = DEFAILT_ARRAY_SIZE;
    char **linesArray = malloc(size * sizeof(char *));
    for (int i = 0; !isEOF; i++) {
        if (size == i) {
            size *= 2;
            linesArray = realloc(linesArray, size * sizeof(char *));
            if (linesArray == NULL) {
                *countOfLines = i + 1;
                goto error;
            }
        }
        char *line = readLine(&isEOF);
        if (line == NULL) {
            *countOfLines = i + 1;
            goto error;
        }
        linesArray[i] = line;
        if (isEOF) {
            *countOfLines = i + 1;
        }
    }
    return linesArray;
error:
    freeArray(linesArray, *countOfLines);
    return NULL;
}

void printArray(char **linesArray, const int countOfLines) {
    for (int i = 0; i < countOfLines; i++) {
        puts(linesArray[i]);
    }
}

char **splitOnWords(const char **line,  const int countOfLines, int *wordCount) {//TODO Доделать функцию

}

char **splitOnUniqWords(const char **linesArray, int *uniqWordCount) {//TODO Доделать функцию

}

int *countNumberOfOccurens(const char **uniqWords, const int uniqWordCount, const char **words, const int wordCount){
    int *numberOfOccurens = malloc(uniqWordCount);
    for (int i = 0; i < uniqWordCount; i++) {
        numberOfOccurens[i] = 0;
        for (int j = 0; j < wordCount; j++) {
            if (strcmp(uniqWords[i], words[j]) == 0) {
                numberOfOccurens[i] += 1;
            }
        }
    }
    return numberOfOccurens;
}

void printFreqOfWords(const char **linesArray,  const int countOfLines) {
    int uniqWordCount = 0;
    int wordCount = 0;
    char **words = splitOnWords(linesArray, countOfLines, &wordCount);
    char **uniqWords = splitOnUniqWords(words, &uniqWordCount);
    int *numberOfOccurens = countNumberOfOccurens(uniqWords, uniqWordCount, words, wordCount);
    //TODO Сделать вывод уникальных слов и их вхождений
    //TODO сделать очистку
}

int main() {
    int countOfLines;
    char **textArray = fillLinesArray(&countOfLines);
    printArray(textArray, countOfLines);
}
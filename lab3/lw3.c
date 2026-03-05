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

char *stringToLower(char *str) {
    int tmpSize = DEFAULT_STR_LEN;
    char *tmp = malloc(tmpSize * sizeof(char));
    if (tmp == NULL) {
        return NULL;
    }
    int i = 0;
    while (str[i] != '\0' ) {
        if (i == tmpSize) {
            tmpSize *= 2;
            tmp = realloc(tmp, tmpSize * sizeof(char));
            if (tmp == NULL) {
                return NULL;
            }
        }
        tmp[i] = (char)tolower(str[i]);
        i++;
    }
    tmp[i] = '\0';
    return tmp;
}

int isFirstOccurens(char **words, const int wordCount, char *word) {
    for (int i = 0; i < wordCount; i++) {
        char *cmpWord1 = stringToLower(word);
        char *cmpWord2 = stringToLower(words[i]);
        if (strcmp(cmpWord1, cmpWord2) == 0) {
            free(cmpWord1);
            free(cmpWord2);
            return i;
        }
        free(cmpWord1);
        free(cmpWord2);
    }
    return -1;
}

char **countNumberOfOccurens(char **words, int *wordsArraySize, int *wordCount, char *word, int *numberOfOccurens, int *numberOfOccurensSize){
    int wordIndex;
    if ((wordIndex = isFirstOccurens(words, *wordCount, word)) != -1) {
        numberOfOccurens[wordIndex] += 1;
    }
    else { //Добавление словва в массив
        wordIndex = *wordCount;
        (*wordCount)++;
        if (*wordsArraySize == *wordCount) {
            char **tempWords = words;
            *wordsArraySize *= 2;
            tempWords = malloc(*wordsArraySize * sizeof(char*));
            words = tempWords;
        }
        words[wordIndex] = word;
        if (*numberOfOccurensSize == *wordCount) {
            int *tempNumberOfOccurens = numberOfOccurens;
            *numberOfOccurensSize *= 2;
            numberOfOccurens = malloc(*numberOfOccurensSize * sizeof(int));
            numberOfOccurens = tempNumberOfOccurens;
        }
        numberOfOccurens[wordIndex] = 1;
    }
    return words;
}

int splitLineOnWordsOccurens(char *line, char **words, int *wordsArraySize, int *uniqWordCount, int *numberOfOccurens, int *numberOfOccurensSize) {
    int i = 0;
    char *tmp = NULL;
    int symbolIndex = 0;

    while (line[i] != '\0') {
        //Прохождение по строке без считки слова
        while (line[i] == ' ' || line[i] == '\t') {
            i++;
        }
        if (line[i] == '\0') {
            break;
        }
        //Инициализация переменных считывания слова
        symbolIndex = 0;
        int lineSize = DEFAULT_STR_LEN;
        tmp = malloc(lineSize * sizeof(char));
        if (tmp == NULL){
            return ENOMEM;
        }
        //Считывание слова
        while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t') {
            if (symbolIndex == lineSize) {
                lineSize *= 2;
                tmp = realloc(tmp, lineSize * sizeof(char));
                if (tmp == NULL) {
                    free(tmp);
                    return ENOMEM;
                }
            }
            tmp[symbolIndex++] = line[i++];
        }
        tmp[symbolIndex] = '\0';
        //Счёт слов
        words = countNumberOfOccurens(words, wordsArraySize, uniqWordCount, tmp, numberOfOccurens, numberOfOccurensSize);
    }
    return 0;
}

char **splitFileOnWordsOccurens(char **linesArray,  const int countOfLines, int *uniqWordCount, int *numberOfOccurens, int *numberOfOccurensSize ) {
    int wordsArraySize = DEFAILT_ARRAY_SIZE;
    *uniqWordCount = 0;
    char **words = malloc(wordsArraySize * sizeof(char*));
    for (int i = 0; i < countOfLines; i++) {
        if (splitLineOnWordsOccurens(linesArray[i], words, &wordsArraySize, uniqWordCount, numberOfOccurens, numberOfOccurensSize) == ENOMEM) {
            freeArray(words, *uniqWordCount);
            return NULL;
        }
    }
    return words;
}

void printOccurensyInfo(char **uniqWords, const int uniqWordCount, int *numberOfOccurens) {
    for (int i = 0; i < uniqWordCount; i++) {
        printf("%s - %d\n", uniqWords[i], numberOfOccurens[i]);
    }
}

void lexicoSort(char **uniqWords, const int uniqWordCount, int *numberOfOccurens) {
    for (int i = 0; i < uniqWordCount - 1; i++) {
        for (int j = i + 1; j < uniqWordCount; j++) {
            char *cmpWord1 = stringToLower(uniqWords[i]);
            char *cmpWord2 = stringToLower(uniqWords[j]);
            if (strcmp(cmpWord1, cmpWord2) < 0) {
                char *tmpWord = uniqWords[i];
                uniqWords[i] = uniqWords[j];
                uniqWords[j] = tmpWord;
                int tmpValue = numberOfOccurens[i];
                numberOfOccurens[i] = numberOfOccurens[j];
                numberOfOccurens[j] = tmpValue;
            }
            free(cmpWord1);
            free(cmpWord2);
        }
    }
}

void printFreqOfWords(char **linesArray,  const int countOfLines) {
    int uniqWordCount;
    int numberOfOccurensSize = DEFAILT_ARRAY_SIZE;
    int *numberOfOccurens = malloc(numberOfOccurensSize * sizeof(int)); //TODO Не забыть освободить память
    char **uniqWords = splitFileOnWordsOccurens(linesArray, countOfLines, &uniqWordCount, numberOfOccurens, &numberOfOccurensSize);
    lexicoSort(uniqWords, uniqWordCount, numberOfOccurens);
    printOccurensyInfo(uniqWords, uniqWordCount, numberOfOccurens);
    freeArray(uniqWords, uniqWordCount);
    free(numberOfOccurens);
}

int main() {
    int countOfLines;
    char **textArray = fillLinesArray(&countOfLines);
    printFreqOfWords(textArray, countOfLines);
}
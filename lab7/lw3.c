#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my-str.c"

constexpr int DEFAULT_ARRAY_SIZE = 1;

typedef struct {
    char* name;
    int quantity;
    float price;
} Product;

void FreeGoods(Product** goodsArray, const int goodsArraySize)
{
    if (goodsArray != NULL)
    {
        for (int i = 0; i < goodsArraySize; i++)
        {
            free(goodsArray[i]->name);
        }
        free(goodsArray);
    }
}

int ReadFileLine(FILE* file, char** line)
{
    *line = NULL;
    char ch;
    size_t index = 0;
    int size = DEFAULT_STR_LEN;
    char* buff = malloc(size);
    if (buff == NULL)
    {
        goto error;
    }
    while ((ch = fgetc(file)) != '\n' && ch != EOF)
    {
        if (file == NULL)
        {
            goto error;
        }
        if (index == size)
        {
            size *= 2;
            buff = realloc(*line, size);
            if (buff == NULL)
            {
                goto error;
            }
        }
        *line = buff;
        buff[index++] = ch;
    }
    size += 1;
    buff = realloc(*line, size);
    if (buff == NULL)
    {
        goto error;
    }
    buff[index] = '\0';
    *line = buff;
    if (ch == EOF)
    {
        return EOF;
    }
    return 0;
    error:
        free(*line);
    return 1;
}

void PutGoods(Product* goodsArray, const char* CSVLine, const int index)
{
    char* lineCopy = malloc(strlen(CSVLine) + 1);
    strcpy(lineCopy, CSVLine);

    char* token = strtok(lineCopy, ",");
    if (token)
    {
        goodsArray[index].name = malloc(strlen(token) + 1);
        strcpy(goodsArray[index].name, token);
    }

    token = strtok(NULL, ",");
    if (token)
    {
        goodsArray[index].quantity = atoi(token);
    }

    token = strtok(NULL, ",");
    if (token)
    {
        goodsArray[index].price = atof(token);
    }

    free(lineCopy);
}

int CheckCSVLine(const char* CSVLine)
{
    int commaCount = 0;
    int index = 0;
    char ch = '0';
    while (ch != '\0')
    {
        ch = CSVLine[index++];
        if (ch == ',')
        {
            commaCount += 1;
        }
    }
    if (commaCount == 2)
    {
        return 0;
    }
    return 1;
}

int ParseGoodsCSV(Product** goodsArray, const int argc, const char* argv[], int* goodsArraySize)
{
    if (argc <= 1)
    {
        perror("Ошибка: не указан CSV файл");
        return 1;
    }

    FILE* CSVFile = fopen(argv[1], "r");
    if (CSVFile == NULL)
    {
        perror("Ошибка открытия файла");
        return 1;
    }

    char* CSVLine = NULL;
    int status;
    int index = 0;
    *goodsArraySize = DEFAULT_ARRAY_SIZE;

    *goodsArray = malloc(*goodsArraySize * sizeof(Product));
    if (*goodsArray == NULL)
    {
        fclose(CSVFile);
        return 1;
    }

    do
    {
        status = ReadFileLine(CSVFile, &CSVLine);
        if (status == 1)
        {
            perror("Ошибка чтения файла");
            fclose(CSVFile);
            if (CSVFile == NULL)
            {
                perror("Ошибка закрытия файла");
            }
            FreeGoods(goodsArray, *goodsArraySize);
            return 1;
        }

        if (CheckCSVLine(CSVLine) == 1)
        {
            fclose(CSVFile);
            if (CSVFile == NULL)
            {
                perror("Ошибка закрытия");
            }
            perror("Неверный формат CSV файла");
            FreeGoods(goodsArray, *goodsArraySize - 1);
            return 1;
        }

        if (index == *goodsArraySize)
        {
            *goodsArraySize *= 2;
            Product* newArray = realloc(*goodsArray, *goodsArraySize * sizeof(Product));
            if (newArray == NULL)
            {
                fclose(CSVFile);
                if (CSVFile == NULL)
                {
                    perror("Ошибка закрытия");
                }
                FreeGoods(goodsArray, *goodsArraySize);
                return 1;
            }
            *goodsArray = newArray;
        }

        PutGoods(*goodsArray, CSVLine, index);
        index++;
        free(CSVLine);
        CSVLine = NULL;
    } while (status != EOF);

    *goodsArraySize = index;
    fclose(CSVFile);
    return 0;
}

char* LowerWord(const char* word)
{
    const int size = strlen(word);
    char* strLowerCase = malloc(size + 1);
    for (int i = 0; i < size; i++)
    {
        strLowerCase[i] = tolower(word[i]);
    }
    strLowerCase[size] = '\0';
    return strLowerCase;
}

void PrintMatches(char* key, const Product* goodsArray, const int goodsArraySize)
{
    int found = 0;
    float totalCost = 0;
    for (int i = 0; i < goodsArraySize; i++)
    {
        const char* result = strstr(LowerWord(goodsArray[i].name), LowerWord(key));
        if (result != NULL)
        {
            printf("Найден: %s, %d шт., %.2f руб.\n",
                   goodsArray[i].name,
                   goodsArray[i].quantity,
                   goodsArray[i].price);
            totalCost += goodsArray[i].quantity * goodsArray[i].price;
            found++;
        }
    }
    if (found == 0)
    {
        printf("Товар '%s' не найден\n", key);
    }
    else
    {
        printf("Total: %.2f", &totalCost);

    }
}

int main(const int argc, const char* argv[])
{
    Product* goodsArray = NULL;
    int goodsArraySize = 0;

    if (ParseGoodsCSV(&goodsArray, argc, argv, &goodsArraySize) == 0)
    {
        char* key = ReadLine();
        while (key != NULL && strcmp(key, "") != 0)
        {
            PrintMatches(key, goodsArray, goodsArraySize);
            free(key);
            key = ReadLine();
        }
        free(key);
        FreeGoods(&goodsArray, goodsArraySize);
        return 0;
    }
    return 1;
}
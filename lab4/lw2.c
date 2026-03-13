#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

const int DEFAULT_STR_LEN = 4;
const char *INPUT_ERROR = "Ошибка ввода!";
const char *MEMORY_ERROR = "Недостаточно памяти!";

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    Point point;
    int distance;
    char *name;
} City;

char *readLine()
{
    char *s = NULL;
    size_t len = 0;
    int chCode;
    int size = DEFAULT_STR_LEN;
    char *tmp = malloc(size);
    if (tmp == NULL)
    {
        goto error;
    }
    while ((chCode = getchar()) != '\n' && chCode != EOF)
    {
        if (len == size)
        {
            size *= 2;
            tmp = realloc(s, size);
        }
        if (tmp == NULL)
        {
            goto error;
        }
        s = tmp;
        s[len++] = (char)chCode;
    }
    tmp = realloc(s, len + 1);
    if (tmp == NULL)
    {
        goto error;
    }
    s = tmp;
    s[len] = '\0';
    return s;
error:
    free(s);
    return NULL;
}

void freeCitiesArray(City *citiesArray, const int countOfCities)
{
    for (int i = 0; i < countOfCities; i++)
    {
        free(citiesArray[i].name);
    }
    free(citiesArray);
}

City *readCities(const int countOfCities)
{
    City *citiesArray = malloc(countOfCities * sizeof(City));
    for (int i = 0; i < countOfCities; i++)
    {
        if (scanf("%d %d", &citiesArray[i].point.x, &citiesArray[i].point.y) == -1)
        {
            goto inputError;
        }
        getchar();
        if ((citiesArray[i].name = readLine()) == NULL)
        {
            goto memoryError;
        }
    }
    return citiesArray;
memoryError:
    return NULL;
inputError:
    puts(INPUT_ERROR);
    return NULL;
}

int readPoint(Point *p)
{
    if (scanf("%d %d", &p->x, &p->y) == -1)
    {
        goto inputError;
    }
    return 0;
inputError:
    puts(INPUT_ERROR);
    return EBADMSG;
}

void printCity(City c)
{
    printf("%s: X: %d Y: %d\n", c.name, c.point.x, c.point.y);
}

void printPoint(Point p)
{
    printf("X: %d Y: %d", p.x, p.y);
}

int main(void)
{
    int countOfCities;
    int isCorrectInput = 0;
    if (scanf("%d", &countOfCities) == -1)
    {
        goto inputError;
    }
    City *citiesArray;
    Point userPoint;
    if ((citiesArray = readCities(countOfCities)) == NULL)
    {
        goto memoryError;
    }
    if (readPoint(&userPoint) == EBADMSG)
    {
        goto inputError;
    }
    //ОТЛАДОЧНЫЙ ВЫВОД
    for (int i = 0; i < countOfCities; i++)
    {
        printCity(citiesArray[i]);
    }
    printPoint(userPoint);
    //TODO У нас есть считанные данные, теперь их надо обработать
    freeCitiesArray(citiesArray, countOfCities);
    return 0;
memoryError:
    puts(MEMORY_ERROR);
    freeCitiesArray(citiesArray, countOfCities);
    return 1;
inputError:
    puts(INPUT_ERROR);
    return 1;
}
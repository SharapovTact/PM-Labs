#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

const int DEFAULT_STR_LEN = 4;

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    Point point;
    char *name;
} City;

char *readLine() {
    char *s = NULL;
    size_t len = 0;
    int chCode;
    int size = DEFAULT_STR_LEN;
    char *tmp = malloc(size);
    if (tmp == NULL) {
        goto error;
    }
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
        scanf("%d %d", &citiesArray[i].point.x, &citiesArray[i].point.y);
        getchar();
        if ((citiesArray[i].name = readLine()) == NULL)
        {
            goto error;
        }
    }
    return citiesArray;
error:
    freeCitiesArray(citiesArray, countOfCities);
    return NULL;
}

Point readPoint()
{
    Point p;
    scanf("%d %d", &p.x, &p.y);
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
    scanf("%d", &countOfCities);
    City *citiesArray;
    Point userPoint;
    if ((citiesArray = readCities(countOfCities)) == NULL)
    {
        goto error;
    }
    userPoint = readPoint();
    //ОТЛАДОЧНЫЙ ВЫВОД
    for (int i = 0; i < countOfCities; i++)
    {
        printCity(citiesArray[i]);
    }
    printPoint(userPoint);
    //TODO У нас есть считанные данные, теперь их надо обработать
    return 0;
error:
    puts("Недостаточно памяти!");
    return 1;
}
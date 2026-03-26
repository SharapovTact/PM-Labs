#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

constexpr int DEFAULT_STR_LEN = 4;
const char *INPUT_ERROR = "ERROR INPUT!";
const char *MEMORY_ERROR = "NO MEMORY!";

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
    if (s != NULL)
    {
        free(s);
    }
    return NULL;
}

void freeCitiesArray(City *citiesArray, const int countOfCities)
{
    for (int i = 0; i < countOfCities && citiesArray != NULL; i++)
    {
        free(citiesArray[i].name);
    }
    if (citiesArray != NULL)
    {
        free(citiesArray);
    }
}

int readCities(City **citiesArray, const int countOfCities)
{
    if (countOfCities == 0)
    {
        return EBADMSG;
    }
    *citiesArray = malloc(countOfCities * sizeof(City));
    if (*citiesArray == NULL)
    {
        return ENOMEM;
    }

    for (int i = 0; i < countOfCities; i++)
    {
        if (scanf("%d %d", &(*citiesArray)[i].point.x, &(*citiesArray)[i].point.y) == -1)
        {
            return EBADMSG;
        }
        getchar();
        if (((*citiesArray)[i].name = readLine()) == NULL)
        {
            return ENOMEM;
        }
    }
    return 0;
}

int readPoint(Point *p)
{
    if (scanf("%d %d", &p->x, &p->y) == -1)
    {
        return EBADMSG;
    }
    return 0;
}

void printCity(City c)
{
    printf("%s (%d; %d) Distance = %d\n", c.name, c.point.x, c.point.y, c.distance);
}

void printPoint(Point p)
{
    printf("X: %d Y: %d", p.x, p.y);
}

int calcDistanceFromUserToCity(const Point userPoint, City *city)
{
    city->distance = abs(userPoint.x - city->point.x) + abs(userPoint.y - city->point.y);
}

void countDistance(const Point userPoint, City *citiesArray, const int countOfCities)
{
    for (int i = 0; i < countOfCities; i++)
    {
        calcDistanceFromUserToCity(userPoint, &citiesArray[i]);
    }
}

int compareCity(const void *sort1, const void *sort2)
{
    const int sortI1 = ((const City*)sort1)->distance;
    const int sortJ2 = ((const City*)sort2)->distance;
    if (sortI1 < sortJ2)
    {
        return -1;
    }
    if (sortI1 == sortJ2)
    {
        return 0;
    }
    return 1;
}

int sortByDistance(City *citiesArray, const int countOfCities)
{
    qsort(citiesArray, countOfCities, sizeof(City), compareCity);
}

void printNearestCities(const City *citiesArray, const int countOfCities)
{
    const int minDistance = citiesArray[0].distance;
    puts("Nearest:");
    for (int i = 0; i < countOfCities; i++)
    {
        if (citiesArray[i].distance == minDistance)
        {
            printCity(citiesArray[i]);
        }
    }
}

int main(void)
{
    int countOfCities;
    City *citiesArray = NULL;
    if (scanf("%d", &countOfCities) == -1)
    {
        goto inputError;
    }
    Point userPoint;
    switch (readCities(&citiesArray, countOfCities))
    {
        case ENOMEM:
            goto memoryError;
        case EBADMSG:
            goto inputError;
        default:
            break;
    }
    if (readPoint(&userPoint) == EBADMSG)
    {
        goto inputError;
    }

    countDistance(userPoint, citiesArray, countOfCities);
    sortByDistance(citiesArray, countOfCities);
    printNearestCities(citiesArray, countOfCities);

    freeCitiesArray(citiesArray, countOfCities);
    return 0;
memoryError:
    puts(MEMORY_ERROR);
    freeCitiesArray(citiesArray, countOfCities);
    return ENOMEM;
inputError:
    puts(INPUT_ERROR);
    freeCitiesArray(citiesArray, countOfCities);
    return EBADMSG;
}
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

int readVertex(Point **vertexArray, const int countOfVertex)
{
    *vertexArray = malloc(countOfVertex * sizeof(Point));
    if (*vertexArray == NULL) {
        return ENOMEM;
    }

    for (int i = 0; i < countOfVertex; i++)
    {
        if (scanf("%d %d", &(*vertexArray)[i].x, &(*vertexArray)[i].y) == -1)
        {
            return EBADMSG;
        }
    }
    return 0;
}

int readPoint(Point *p)
{
    if (scanf("%d %d", &p->x, &p->y) == -1)
    {
        goto inputError;
    }
    return 0;
inputError:
    return EBADMSG;
}

void printVertex(Point p)
{
    printf("(%d; %d)\n", p.x, p.y);
}

int main(void)
{
    int countOfVertex;
    Point *vertexArray;
    if (scanf("%d", &countOfVertex) == -1)
    {
        goto inputError;
    }
    switch (readVertex(&vertexArray, countOfVertex))
    {
        case ENOMEM:
            goto memoryError;
        case EBADMSG:
            goto inputError;
        default:
            break;
    }

    //ОТЛАДОЧНЫЙ ВЫВОД
    for (int i = 0; i < countOfVertex; i++)
    {
        printVertex(vertexArray[i]);
    }

    free(vertexArray);
    return 0;
memoryError:
    puts(MEMORY_ERROR);
    free(vertexArray);
    return ENOMEM;
inputError:
    puts(INPUT_ERROR);
    free(vertexArray);
    return EBADMSG;
}
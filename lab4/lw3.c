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
    int countOfVertex;
    Point *vertices;
} Octagon;

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

int findCross(Point p1, Point p2, Point p3)
{
    return (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
}

bool isConvex(const Octagon* oct)
{
    bool isConvex = false;
    for (int i = 0; i < oct->countOfVertex; i++)
    {
        Point p1 = oct->vertices[i];
        Point p2 = oct->vertices[(i + 1) % oct->countOfVertex];
        Point p3 = oct->vertices[(i + 2) % oct->countOfVertex];
        int cross = findCross(p1, p2, p3);
        if (cross > 0)
        {
            isConvex = true;
        }
        if (cross < 0)
        {
            return false;
        }
    }
    return isConvex;
}

int inputOctagon(Octagon *oct)
{
    int countOfVertex;
    if (scanf("%d", &countOfVertex) == -1 || countOfVertex < 3)
    {
        return EBADMSG;
    }
    (*oct).countOfVertex = countOfVertex;
    switch (readVertex(&(*oct).vertices, (*oct).countOfVertex))
    {
        case ENOMEM:
            return ENOMEM;
        case EBADMSG:
            return EBADMSG;
        default:
            break;
    }
    return 0;
}

int calcParticiallySquare(const Octagon *oct)
{
    int sum = 0;
    for (int i = 0; i < oct->countOfVertex; i++)
    {
        int j = (i + 1) % oct->countOfVertex;
        Point p1 = oct->vertices[i];
        Point p2 = oct->vertices[j];
        sum += p1.x * p2.y - p1.y * p2.x;
    }

    return sum;
}

void printOctagSquare(const Octagon *oct)
{
    int sum = calcParticiallySquare(oct);
    printf("Square is: %d", sum / 2);
    if (sum % 2 != 0)
    {
        printf(".5");
    }
    puts("");
}

int main(void)
{
    Octagon oct;
    switch (inputOctagon(&oct))
    {
        case ENOMEM:
            goto memoryError;
        case EBADMSG:
            goto inputError;
        default:
            break;
    }
    if (isConvex(&oct))
    {
        puts("Convex");
        printOctagSquare(&oct);
    }
    else
    {
        puts("Not convex");
    }
    free(oct.vertices);
    return 0;
memoryError:
    puts(MEMORY_ERROR);
    free(oct.vertices);
    return ENOMEM;
inputError:
    puts(INPUT_ERROR);
    free(oct.vertices);
    return EBADMSG;
}
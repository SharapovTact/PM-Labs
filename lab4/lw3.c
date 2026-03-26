#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

constexpr int DEFAULT_STR_LEN = 4;
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
} Polygon;

void PrintVertex(Point p)
{
    printf("(%d; %d)\n", p.x, p.y);
}

int ReadVertex(Point **vertexArray, const int countOfVertex)
{
    *vertexArray = malloc(countOfVertex * sizeof(Point));
    if (*vertexArray == NULL)
    {
        return ENOMEM;
    }

    for (int i = 0; i < countOfVertex; i++)
    {
        int x;
        int y;
        if (scanf("%d %d", &x, &y) == -1)
        {
            return EBADMSG;
        }
        (*vertexArray)[i].x = abs(x);
        (*vertexArray)[i].y = abs(y);
        PrintVertex((*vertexArray)[i]);
    }
    return 0;
}

int FindCross(const Point p1, const Point p2, const Point p3)
{
    return abs(abs(p2.x - p1.x) * abs(p3.y - p2.y) - abs(p2.y - p1.y) * abs(p3.x - p2.x));
}

bool IsConvex(const Polygon* poly)
{
    bool isConvex = false;
    for (int i = 0; i < poly->countOfVertex; i++)
    {
        Point p1 = poly->vertices[i];
        Point p2 = poly->vertices[(i + 1) % poly->countOfVertex];
        Point p3 = poly->vertices[(i + 2) % poly->countOfVertex];
        int cross = FindCross(p1, p2, p3);
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

int InputPolygon(Polygon *poly)
{
    int countOfVertex;
    if (scanf("%d", &countOfVertex) == -1 || countOfVertex < 3)
    {
        return EBADMSG;
    }
    (*poly).countOfVertex = countOfVertex;
    switch (ReadVertex(&(*poly).vertices, (*poly).countOfVertex)) //TODO изменить названия
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

int CalcParticiallySquare(const Polygon *poly)
{
    int sum = 0;
    for (int i = 0; i < poly->countOfVertex; i++)
    {
        int j = (i + 1) % poly->countOfVertex;
        Point p1 = poly->vertices[i];
        Point p2 = poly->vertices[j];
        sum += p1.x * p2.y - p1.y * p2.x;
    }

    return sum;
}

void PrintPolySquare(const Polygon *poly)
{
    const int sum = CalcParticiallySquare(poly);
    printf("Square is: %d", sum / 2);
    if (sum % 2 != 0)
    {
        printf(".5");
    }
    puts("");
}

int main(void)
{
    Polygon poly;
    switch (InputPolygon(&poly))
    {
        case ENOMEM:
            goto memoryError;
        case EBADMSG:
            goto inputError;
        default:
            break;
    }
    if (IsConvex(&poly))
    {
        puts("Convex");
        PrintPolySquare(&poly);
    }
    else
    {
        puts("Not convex");
    }
    free(poly.vertices);
    return 0;
memoryError:
    puts(MEMORY_ERROR);
    free(poly.vertices);
    return ENOMEM;
inputError:
    puts(INPUT_ERROR);
    free(poly.vertices);
    return EBADMSG;
}
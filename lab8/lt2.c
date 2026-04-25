#include "shapes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int DEFAULT_STR_LEN = 4;

char *ReadWord()
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
    while ((chCode = getchar()) != ' ' && chCode != EOF)
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

int CircleParse(Shape** shapes, int* countShape)
{
    double x, y;
    double rad;
    if (scanf(" %lf %lf %lf", &x, &y, &rad) != 3)
    {
        goto error;
    }
    uint8_t r, g, b;
    if (scanf(" #%02x%02x%02x", &r, &g, &b) != 3)
    {
        goto error;
    }

    shapes[*countShape] = CreateCircle((Color){r, g, b}, (Point){x, y}, rad);
    *countShape += 1;
    return 0;
error:
    perror("Ошибка считывания данных круга");
    return 1;
}

int RectangleParse(Shape** shapes, int* countShape)
{
    double x, y;
    double width, height;
    if (scanf(" %lf %lf %lf %lf", &x, &y, &width, &height) != 4)
    {
        goto error;
    }
    if (width <= 0 || height <= 0)
    {
        goto error;
    }
    uint8_t r, g, b;
    if (scanf(" #%02x%02x%02x", &r, &g, &b) != 3)
    {
        goto error;
    }

    shapes[*countShape] = CreateRectangle((Color){r, g, b}, (Point){x, y}, width, height);
    *countShape += 1;
    return 0;
error:
    perror("Ошибка считывания данных прямоугольника");
    return 1;
}

int TriangleParse(Shape** shapes, int* countShape)
{
    double x1, y1;
    double x2, y2;
    double x3, y3;
    if (scanf(" %lf %lf %lf %lf %lf %lf", &x1, &y1, &x2, &y2, &x3, &y3) != 6)
    {
        goto error;
    }
    const double cross = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
    if (cross == 0) {
        goto error;
    }
    uint8_t r, g, b;
    if (scanf(" #%02x%02x%02x", &r, &g, &b) != 3)
    {
        goto error;
    }

    shapes[*countShape] = CreateTriangle((Color){r, g, b}, (Point){x1, y1}, (Point){x2, y2}, (Point){x3, y3});
    *countShape += 1;
    return 0;
    error:
        perror("Ошибка считывания данных треугольника");
    return 1;
}

//SuperFree как-то непонятно
void FreeShapesArray(Shape** shapes, const int countShape)
{
    if (shapes != NULL)
    {
        for (int i = 0; i < countShape; i++)
        {
            free(shapes[i]);
        }
        free(shapes);
    }
}

int CompareAreaIncreasing(const void* a, const void* b)
{
    const double areaA = ShapeGetArea(*(const Shape**)a);
    const double areaB = ShapeGetArea(*(const Shape**)b);

    if (areaA < areaB)
    {
        return -1;
    }
    if (areaA > areaB)
    {
        return 1;
    }
    return 0;
}

int ComparePerimeterDecreasing(const void* a, const void* b)
{
    const double perimeterA = ShapeGetPerimeter(*(const Shape**)a);
    const double perimeterB = ShapeGetPerimeter(*(const Shape**)b);

    if (perimeterA > perimeterB)
    {
        return -1;
    }
    if (perimeterA < perimeterB)
    {
        return 1;
    }
    return 0;
}

int ReadNum()
{
    int num;
    printf("Введите кол-во фигур: ");
    if (scanf("%d", &num) != 1)
    {
        perror("Ошибка ввода кол-ва фигур");
        return 0;
    }
    if (num < 1)
    {
        perror("Ввод меньше единицы");
        return 0;
    }
    return num;
}

int main(void) {
    int numShapes = ReadNum();
    if (numShapes == 0)
    {
        return 0;
    }
    int countShape = 0;
    Shape **shapes = malloc(numShapes * sizeof(*shapes));
    for (int i = 0; i < numShapes; i++)
    {
        scanf("\n");
        char* shapeName = ReadWord();
        if (strcmp(shapeName, "circle") == 0)
        {
            if (CircleParse(shapes, &countShape) == 1) //TODO лучше чтобы я передавал строку, возвращал фигуру и запихивал в массив уже здесь
            {
                goto errorFree;
            }
        }
        else if (strcmp(shapeName, "rectangle") == 0)
        {
            if (RectangleParse(shapes, &countShape) == 1)
            {
                goto errorFree;
            }
        }
        else if (strcmp(shapeName, "triangle") == 0)
        {
            if (TriangleParse(shapes, &countShape) == 1)
            {
                goto errorFree;
            }
        }
        else
        {
            perror("Ошибка ввода фигуры");
            free(shapeName);
            goto errorFree;
        }
        free(shapeName);
    }

    qsort(shapes, numShapes, sizeof(*shapes), CompareAreaIncreasing);
    for (int i = 0; i < numShapes; i++) {
        ShapeDraw(shapes[i]);
        printf(", Area = %.2f", ShapeGetArea(shapes[i]));
        printf(", Perimeter = %.2f\n", ShapeGetPerimeter(shapes[i]));
    }

    qsort(shapes, numShapes, sizeof(*shapes), ComparePerimeterDecreasing);
    for (int i = 0; i < numShapes; i++) {
        ShapeDraw(shapes[i]);
        printf(", Area = %.2f", ShapeGetArea(shapes[i]));
        printf(", Perimeter = %.2f\n", ShapeGetPerimeter(shapes[i]));
    }

    FreeShapesArray(shapes, countShape);
    return 0;
errorFree:
    FreeShapesArray(shapes, countShape);
    return 1;
}

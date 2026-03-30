#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    double x;
    double y;
} DoublePoint;

typedef struct
{
    int x;
    int y;
} Ray;

typedef struct
{
    Point pos;
    double r;
    DoublePoint posInter;
    double intersectionDistance;
    bool isIntersection;
    int time;
} Iland;

typedef struct
{
    Point pos;
    Ray direction;
} Raft;

typedef struct
{
    Raft raft;
    int ilandsCount;
    Iland *ilands;
} World;

int InputWorld(World *world)
{
    printf("Введите координаты плота x y: ");
    if (scanf("%d%d", &world->raft.pos.x, &world->raft.pos.y) == -1)
    {
        goto inputError;
    }
    printf("Введите направление плота x y: ");
    if (scanf("%d%d", &world->raft.direction.x, &world->raft.direction.y) == -1)
    {
        goto inputError;
    }
    printf("Введите кол-во островов: ");
    if (scanf("%d", &world->ilandsCount) == -1)
    {
        goto inputError;
    }
    world->ilands = malloc(sizeof(Iland) * world->ilandsCount);//TODO Очистить память
    if (world->ilands == NULL)
    {
        goto memoryError;
    }
    for (int i = 0; i < world->ilandsCount; i++)
    {
        printf("Введите данные острова x y r: ");
        if (scanf("%d%d%lf", &world->ilands[i].pos.x, &world->ilands[i].pos.y, &world->ilands[i].r) == -1)
        {
            goto inputError;
        }
        world->ilands[i].intersectionDistance = 0.0;
        world->ilands[i].isIntersection = false;
    }
    return 0;
inputError:
    puts("InputError!");
    return EBADMSG;
memoryError:
    puts("MemoryError!");
    return ENOMEM;
}

void PrintIntersection(const World world)
{
    int firstIntersectionIland = -1;
    double minIntersectionDistance = INT_MAX;
    for (int i = 0; i < world.ilandsCount; i++)
    {
        if (world.ilands[i].isIntersection && world.ilands[i].intersectionDistance < minIntersectionDistance)
        {
            firstIntersectionIland = i;
            minIntersectionDistance = world.ilands[i].intersectionDistance;
        }
    }
    if (world.raft.direction.x == 0 && world.raft.direction.y == 0 && firstIntersectionIland == -1)
    {
        printf("Остановился, отдохнуть, скоро его сожрут акулы, пу-пу-пу..");
    }
    else if (firstIntersectionIland == -1)
    {
        printf("Уплывает в бесконечное плаванье, помянем..");
    }
    else {
        printf("Направляется на остров №%d, дистанция - %lf, через %dч, на позицию: %lf %lf", firstIntersectionIland,
            minIntersectionDistance, world.ilands[firstIntersectionIland].time, world.ilands[firstIntersectionIland].posInter.x,
            world.ilands[firstIntersectionIland].posInter.y);
    }
}

double FindT(const double A, const double B, const double C)
{
    double D = pow(B, 2) - 4 * A * C;
    if (D < 0)
    {
        return -1;
    }
    if (D == 0)
    {
        return B * -1 / (2 * A);
    }
    if (D > 0)
    {
        const double x1 = (B * -1 + sqrt(D)) / (2 * A);
        const double x2 = (B * -1 - sqrt(D)) / (2 * A);
        if (x1 < 0)
        {
            return x2;
        }
        if (x2 < 0)
        {
            return x1;
        }
        if (x1 < x2)
        {
            return x1;
        }
        return x2;
    }
}

void FindIntersection(Iland *iland, const Raft raft)
{
    const double dX = raft.direction.x;
    const double dY = raft.direction.y;
    const double delX = raft.pos.x - iland->pos.x;
    const double delY = raft.pos.y - iland->pos.y;
    if (sqrt(pow(delX, 2) + pow(delY, 2)) <= iland->r)
    {
        iland->intersectionDistance = 0;
        iland->isIntersection = true;
        return;
    }
    const double A = pow(dX, 2) + pow(dY, 2);
    const double B = 2 * (delX * dX + delY * dY);
    const double C = pow(delX, 2) + pow(delY, 2) - pow(iland->r, 2);
    const double t = FindT(A, B, C);
    if (t >= 0)
    {
        iland->isIntersection = true;
        double lenDirectionVector = sqrt(pow(dX, 2) + pow(dY, 2));
        iland->posInter.x = raft.pos.x + t * dX;
        iland->posInter.y = raft.pos.y + t * dY;
        iland->intersectionDistance = t * lenDirectionVector;
        iland->time = t / lenDirectionVector;
    }
}

void FindIntersections(World *world)
{
    for (int i = 0; i < world->ilandsCount; i++)
    {
        FindIntersection(&world->ilands[i], world->raft);
    }
}

int main(void)
{
    World world;
    if (InputWorld(&world) != 0)
    {
        return 1;
    }
    FindIntersections(&world);
    PrintIntersection(world);
    free(world.ilands);
}
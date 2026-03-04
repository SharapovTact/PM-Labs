#include <stdio.h>

typedef struct {
    int x;
    int y;
} Point;

Point PrintPoint(Point *p) {
    p->y = 0; // Изменяется копия
    printf("%d %d\n", p->x, p->y);
}

int main(void) {
    Point p1 = {3, 5};
    PrintPoint(&p1);
    printf("%d %d\n", p1.x, p1.y);
}
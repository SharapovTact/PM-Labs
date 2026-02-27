#include <stdio.h>

int Fib(int n) {
    int prev = 0, prev_prev = 0;
    for (int i = 0; i <= n; i++)
    {
        const int temp = prev_prev;
        prev_prev = prev;
        if (i == 0)
        {
            prev = 0;
        }
        else if (i == 1)
        {
            prev = 1;
        }
        else
        {
            prev = prev + temp;
        }
    }
    return prev;
}
int main(void) {
    int n;
    while (scanf("%d", &n) != 1 || n < 0)
    {
        printf("Ошибка ввода, на вход должно подаваться число большее или равное 0\n");
        return 1;
    }
    printf("%d\n", Fib(n));
    return 0;
}
//
#include <stdio.h>
int findMin(int num1, int num2)
{
    if (num1 < num2)
    {
        return num1;
    }
    return num2;

}
int findMax(int num1, int num2)
{
    if (num1 > num2)
    {
        return num1;
    }
    return num2;
}
int isEven(int num)
{
    if (num % 2 == 0)
    {
        return 1;
    }
    return 0;
}
int main(void)
{
    int num, scanSucces = 0;
    int countNum = 0, sum = 0, min = 0, max = 0, even = 0, odd = 0;
    while ((scanSucces = scanf("%d", &num)) == 1 || getchar() != EOF)
    {
         if (scanSucces)
         {
             countNum++;
             sum += num;
             if (countNum)
             {
                 min = num;
                 max = num;
             }
             min = findMin(num, min);
             max = findMax(num, max);
             if (isEven(num))
             {
                 even++;
             }
             else
             {
                 odd++;
             }
         }
    }
    if (countNum > 0)
    {
        printf("Count: %d\nSum: %d\nMin: %d\nMax: %d\nEven: %d\nOdd: %d\n", countNum, sum, min, max, even, odd);
    }
    else
    {
        printf("Count: %d\nSum: %d\n", countNum, sum);
    }
}
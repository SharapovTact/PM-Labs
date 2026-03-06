#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

typedef struct
{
    int day;
    int month;
    int year;
} Date;

void printDate(Date date)
{
    printf("%d %d %d\n", date.day, date.month, date.year);
}

void printDateArray(Date *dateArray, const int countOfDates)
{
    for (int i = 0; i < countOfDates; i++)
    {
        printDate(dateArray[i]);
    }
}

int checkMonth(Date date)
{
    if (date.month < 12 && date.month > 0)
    {
        return 1;
    }
    return 0;
}

int isLeapYear(int year)
{
    if (year % 400 == 0) return 1;
    if (year % 4 == 0 && year % 100 != 0) return 1;
    return 0;
}

int checkDays(Date date)
{
    if (date.day <= 0) return 0;
    switch (date.month)
    {
        case 2:
            if (isLeapYear(date.year) == 1)
            {
                if (date.day <= 29) return 1;
            }
            else
            {
                if (date.day <= 28) return 1;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if (date.day <= 30) return 1;
            break;
        default:
            if (date.day <= 31) return 1;
            break;
    }
    return 0;
}

int isCorrectDate(Date date)
{
    if (checkDays(date) == 1 && checkMonth(date) == 1)
    {
        return 1;
    }
    return 0;
}

Date *datesRead(const int countOfDates, int *isCorrectInput)
{
    Date *dateArray = malloc(countOfDates * sizeof(Date));
    if (dateArray == NULL)
    {
        goto error;
    }
    for (int i = 0; i < countOfDates; i++)
    {
        scanf("%d%d%d", &dateArray[i].day, &dateArray[i].month, &dateArray[i].year);
        if ((*isCorrectInput = isCorrectDate(dateArray[i])) == 0)
        {
            goto error;
        }
    }
    return dateArray;
error:
    free(dateArray);
    return NULL;
}

int dateCmp(Date date1, Date date2)
{
    if (date1.year > date2.year) return 1;
    if (date1.year < date2.year) return -1;
    if (date1.month > date2.month) return 1;
    if (date1.month < date2.month) return -1;
    if (date1.day > date2.day) return 1;
    if (date1.day < date2.day) return -1;
    return 0;
}

void sortDates(Date *dateArray, const int countOfDates)
{
    for (int i = 0; i < countOfDates - 1; i++)
    {
        for (int j = i + 1; j < countOfDates; j++)
        {
            if (dateCmp(dateArray[j], dateArray[i]) == -1)
            {
                Date tmp = dateArray[i];
                dateArray[i] = dateArray[j];
                dateArray[j] = tmp;
            }
        }
    }
}

int main(void)
{
    int countOfDates;
    int isCorrectInput = 0;
    scanf("%d", &countOfDates);
    Date *dateArray;
    if ((dateArray = datesRead(countOfDates, &isCorrectInput)) == NULL)
    {
        if (isCorrectInput == 0)
        {
            goto invalidInput;
        }
        goto notEnoughMem;
    }
    sortDates(dateArray, countOfDates);
    printDateArray(dateArray, countOfDates);
    free(dateArray);
    return 0;
notEnoughMem:
    puts("Недостаточно памяти!");
    return 1;
invalidInput:
    puts("Ошибка ввода!");
    return 1;
}
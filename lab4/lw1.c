#include <stdio.h>
#include <stdlib.h>
#include <string.h>

constexpr int JAN = 1;
constexpr int FEB = 2;
constexpr int MAR = 3;
constexpr int APR = 4;
constexpr int MAY = 5;
constexpr int JUN = 6;
constexpr int JUL = 7;
constexpr int AUG = 8;
constexpr int SEP = 9;
constexpr int OCT = 10;
constexpr int NOV = 11;
constexpr int DEC = 12;

typedef struct
{
    int day;
    int month;
    int year;
} Date;

void printDate(const Date date)
{
    printf("%d %d %d\n", date.day, date.month, date.year);
}

void printDateArray(const Date *dateArray, const int countOfDates)
{
    for (int i = 0; i < countOfDates; i++)
    {
        printDate(dateArray[i]);
    }
}

int checkMonth(const Date date)
{
    constexpr int MAX_MONTH = 12;
    constexpr int MIN_MONTH = 1;
    if (date.month <= MAX_MONTH && date.month >= MIN_MONTH)
    {
        return 1;
    }
    return 0;
}

int isLeapYear(const int year)
{
    if (year % 400 == 0) return 1;
    if (year % 4 == 0 && year % 100 != 0) return 1;
    return 0;
}

int checkDays(const Date date)
{
    constexpr int COUNT_DAYS_MAX = 31;
    constexpr int COUNT_DAYS_DEFAULT = 30;
    constexpr int COUNT_DAYS_FEB_DEFAULT = 28;
    constexpr int COUNT_DAYS_FEB_LEAP = 29;
    if (date.day <= 0) return 0;
    switch (date.month)
    {
        case FEB:
            if (isLeapYear(date.year) == 1)
            {
                if (date.day <= COUNT_DAYS_FEB_LEAP) return 1;
            }
            else
            {
                if (date.day <= COUNT_DAYS_FEB_DEFAULT) return 1;
            }
            break;
        case APR:
        case JUN:
        case SEP:
        case NOV:
            if (date.day <= COUNT_DAYS_DEFAULT) return 1;
            break;
        case JAN:
        case MAR:
        case MAY:
        case JUL:
        case AUG:
        case OCT:
        case DEC:
            if (date.day <= COUNT_DAYS_MAX) return 1;
            break;
        default:
            break;
    }
    return 0;
}

int isCorrectDate(const Date date)
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
        if (scanf("%d%d%d", &dateArray[i].day, &dateArray[i].month, &dateArray[i].year) == -1)
        {
            *isCorrectInput = 0;
            goto error;
        }
        if ((*isCorrectInput = isCorrectDate(dateArray[i])) == 0)
        {
            goto error;
        }
    }
    return dateArray;
error:
    return NULL;
}

int dateCmp(const Date date1, const Date date2)
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
    if (dateArray == NULL)
    {
        return;
    }
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
    if (scanf("%d", &countOfDates) == -1) {
        goto invalidInput;
    }

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
    free(dateArray);
    return 1;
invalidInput:
    puts("Ошибка ввода!");
    free(dateArray);
    return 1;
}
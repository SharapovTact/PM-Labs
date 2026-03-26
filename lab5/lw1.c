#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

constexpr int DEFAULT_STR_LEN = 16;

typedef struct
{
    char name[51];
    short birthMonth;
    char birthDay;
    int birthYear;
    int64_t salary;

} Employee;

bool IsCorrectSymb(const char ch)
{
    if (ch >= 32 && ch <= 255)
    {
        return true;
    }
    return false;
}

void PrintStrLN(const char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        printf("%c", str[i]);
        i++;
    }
    puts("");
}

void FillGaps(const int countDigits)
{
    for (int i = 0; i < DEFAULT_STR_LEN - countDigits - 1; i++)
    {
        printf("   ");
    }
    if (countDigits < 9)
    {
        printf("  ");
    }
}

void PrintSeparator(const int countDigit)
{
    if (countDigit == DEFAULT_STR_LEN / 2)
    {
        printf("| ");
    }
}

void FillStrForDump(char *str, const unsigned char *p, const int countDigit)
{
    if (IsCorrectSymb(p[countDigit]))
    {
        str[countDigit] = p[countDigit];
    }
    else
    {
        str[countDigit] = '.';
    }
}

void PrintDump(const uint8_t* data, const size_t size);

void CallNext16bDump(char *str, const unsigned char *p, const int countDigit, const size_t size)
{
    if (countDigit == DEFAULT_STR_LEN - 1 && size > DEFAULT_STR_LEN)
    {
        str[countDigit + 1] = '\0';
        PrintStrLN(str);
        PrintDump(&p[countDigit + 1], size - DEFAULT_STR_LEN);
    }
    if (countDigit == size - 1)
    {
        FillGaps(countDigit);
        str[countDigit + 1] = '\0';
        PrintStrLN(str);
    }
}

bool IsEndOfDumpStr(const unsigned char *p, const int countDigit, const size_t size)
{
    if (countDigit == DEFAULT_STR_LEN - 1 && size > DEFAULT_STR_LEN)
    {
        return true;
    }
    if (countDigit == size - 1)
    {
        return true;
    }
    return false;
}

void PrintDump(const uint8_t* data, const size_t size)
{
    const unsigned char *p = (unsigned char *)data;
    printf("adress:%p -> ", data);
    char *str = malloc(DEFAULT_STR_LEN + 1);
    if (str == NULL)
    {
        printf("ENOMEM\n");
        return;
    }

    for (size_t i = 0; i < size && i < DEFAULT_STR_LEN; i++) {
        PrintSeparator(i);
        FillStrForDump(str, p, i);
        printf("%02X ", p[i]);
        CallNext16bDump(str, p, i, size);
        if (IsEndOfDumpStr(p, i, size))
        {
            free(str);
        }
    }
}

void DumpArrayInt() //TODO сделать аннотированный вывод
{
    puts("Array:");
    constexpr int size = 6;
    int numberArray[size] = {0, 1, 2, 3, 4, 5};
    PrintDump((uint8_t *)&numberArray, sizeof(numberArray));
}

void DumpString()
{
    puts("String:");
    char *str = "hello my dear world!";
    PrintDump(str, strlen(str) + 1);
}

void DumpStruct()
{
    puts("Struct:");
    Employee emp = {.birthDay = 4, .birthMonth = 9, .birthYear = 2007, .name = "Nikita", .salary = 10000000};
    PrintDump((uint8_t *)&emp, sizeof(Employee));
    printf("%d", sizeof(Employee));
}

int main(void)
{
    //TODO почему используется Little endian, чем он лучше
    DumpArrayInt();
    puts("");
    DumpString();
    puts("");
    DumpStruct();
    return 0;
}
#include <stdio.h>
int isAcceptSymbol(char ch)
{
    if ((ch >= '0' && ch <= '9') ||
        (ch >= 'a' && ch <= 'z') ||
        (ch >= 'A' && ch <= 'Z'))
    {
        return 1;
    }
    return 0;
}

int main(void)
{
    int inWord = 0;
    int wordCount = 0;
    char ch;
    while ((ch = getchar()) != EOF){
        if (!inWord && isAcceptSymbol(ch))
        {
            inWord = 1;
            wordCount++;
        }
        else if (inWord && !isAcceptSymbol(ch))
        {
            inWord = 0;
        }
    }
    printf("Word count is: %d\n", wordCount);
}
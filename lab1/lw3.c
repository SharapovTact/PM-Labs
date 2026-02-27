#include <stdio.h>

int isWrongInput(char answer)
{
    if (answer == '>' || answer == '<' || answer == '=')
    {
        return 0;
    }
    return 1;
}
int isСontradiction(int low, int high, char answer, int guessNum)
{
    if (guessNum == low && answer == '<')
    {
        return 1;
    }
    if (guessNum == high && answer == '>')
    {
        return 1;
    }
    if (low == high )
    {
        return 1;
    }
    return 0;
}
int getCharBeforeEOLN()
{
    int inpChar, answer;
    while ((inpChar = getchar()) != 10) {
        answer = inpChar;
        inpChar = 10;
    }
    return answer;
}
int main(void)
{
    int low = 0, high = 101, guessNum;
    char answer = '>';
    while (answer != '=' )
    {
        guessNum = (low + high) / 2;
        if (guessNum == low && guessNum != high && high - low == 1 && answer == '>')
        {
            guessNum++;
            low++;
        }
        else if (guessNum != low && guessNum == high && high - low == 1 && answer == '<')
        {
            guessNum++;
            high--;
        }
        if (isСontradiction(low, high, answer, guessNum))
        {
            printf("Ошибка: Ответ противоречив\n");
            return 0;
        }
        //printf("%d %d\n", low, high); {Отладка}
        printf("Я предполагаю: %d\nВаш ответ (<, >, =): ", guessNum);
        answer = getCharBeforeEOLN();
        puts("");
        if (isWrongInput(answer))
        {
            printf("Ошибка ввода, недопустимый символ\n");
        }
        else
        {
            if (answer == '>')
            {
                low = guessNum;
            }
            else if (answer == '<')
            {
                high = guessNum;
            }
        }
    }
    printf("Я угадало! Ваше число: %d\n", guessNum);
    return 0;
}
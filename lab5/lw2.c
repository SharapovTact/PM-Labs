#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my-dump.c"
#include "my-str.c"

constexpr int MAX_KEY_VALUE = 255;
constexpr int MIN_KEY_VALUE = 0;

int ReadKey()
{
    int key;
    if (scanf("%d", &key) == -1)
    {
        goto errorInput;
    }
    if (key < MIN_KEY_VALUE || key > MAX_KEY_VALUE)
    {
        goto errorInput;
    }
    return key;
errorInput:
    puts("Wrong input key, set key = 0");
    return MIN_KEY_VALUE;
}

void StrCopy(const char *str1, char **str2)
{
    const int size = strlen(str1);
    *str2 = malloc(size + 1);
    if (*str2 == NULL)
    {
        puts("Memory error");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        (*str2)[i] = str1[i];
    }
    (*str2)[size] = '\0';
}

unsigned char RotateRight(const unsigned char byte, int shift, const int shiftMax)
{
    shift %= shiftMax;
    return byte >> shift | byte << (shiftMax - shift);
}

unsigned char RotateLeft(const unsigned char byte, int shift, const int maxShift)
{
    shift %= maxShift;
    return byte << shift | byte >> (maxShift - shift);
}

char *Encrypt(const char *rawStr, const int key)
{
    char *cryptedStr = NULL;
    StrCopy(rawStr, &cryptedStr);
    constexpr int maxShift = 8;
    for (int i = 0; i < strlen(rawStr); i++)
    {
        cryptedStr[i] = RotateRight(cryptedStr[i], i, maxShift);
        cryptedStr[i] = cryptedStr[i] ^ key;
    }
    return cryptedStr;
}

char *Decrypt(const char *cryptedStr, const int key)
{
    char *uncryptedStr = NULL;
    StrCopy(cryptedStr, &uncryptedStr);
    constexpr int maxShift = 8;
    for (int i = 0; i < strlen(cryptedStr); i++)
    {
        uncryptedStr[i] = uncryptedStr[i] ^ key;
        uncryptedStr[i] = RotateLeft(uncryptedStr[i], i, maxShift);
    }
    return uncryptedStr;
}

int main(void)
{
    const char *str = ReadLine();
    const int key = ReadKey();
    char *strCrypted = Encrypt(str, key);
    char *strUncrypted = Decrypt(strCrypted, key);
    PrintDump(strCrypted, strlen(strCrypted));
    PrintDump(strUncrypted, strlen(strUncrypted));
    free(strCrypted);
    free(strUncrypted);
    return 0;
}
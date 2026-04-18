#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my-str.c"

int WriteCompressedLastByte(FILE* OutputFile, int byteCount, const char ch)
{
    fputc(byteCount, OutputFile);
    fputc(ch, OutputFile);
    return 0;
}

int WriteCompressedBytes(FILE* OutputFile, int byteCount, const char ch)
{
    for (int i = 0; i < byteCount / 255; i++)
    {
        byteCount -= 255;
        fputc(255, OutputFile);
        fputc(ch, OutputFile);
    }
    if (byteCount > 0)
    {
        return WriteCompressedLastByte(OutputFile, byteCount, ch);
    }
    return 0;
}

int ReadWriteWithCompress(FILE* InputFile, FILE* OutputFile)
{
    int ch;
    if ((ch = fgetc(InputFile)) == EOF) {
        return 0;
    }
    int chPrev = ch;
    int repeatCounter = 1;

    while ((ch = fgetc(InputFile)) != EOF)
    {
        if (ch != chPrev)
        {
            WriteCompressedBytes(OutputFile, repeatCounter, chPrev);
            repeatCounter = 1;
            chPrev = ch;
        }
        else
        {
            repeatCounter++;
        }
    }
    return WriteCompressedBytes(OutputFile, repeatCounter, chPrev);
}

int Compress(const char* argv[])
{
    FILE* InputFile = fopen(argv[2], "r");
    if (InputFile == NULL)
    {
        perror("Ошибка открытия файла");
        return 1;
    }
    FILE* OutputFile = fopen(argv[3], "w");
    if (OutputFile == NULL)
    {
        perror("Ошибка открытия файла");
        return 1;
    }
    int returnCode = ReadWriteWithCompress(InputFile, OutputFile);
    fclose(InputFile);
    fclose(OutputFile);
    return returnCode;
}

int ReadWriteWithDecompress(FILE* InputFile, FILE* OutputFile)
{
    int chCount;
    while ((chCount = fgetc(InputFile)) != EOF)
    {
        const char ch = fgetc(InputFile);
        if (ch == EOF) {
            perror("Ошибка: нечетное количество байт");
            return 1;
        }
        for (int i = 0; i < chCount; i++)
        {
            fputc(ch, OutputFile);
        }
    }
    return 0;
}

int Decompress(const char* argv[])
{
    FILE* InputFile = fopen(argv[2], "r");
    if (InputFile == NULL)
    {
        perror("Ошибка открытия файла");
        return 1;
    }
    FILE* OutputFile = fopen(argv[3], "w");
    if (OutputFile == NULL)
    {
        perror("Ошибка открытия файла");
        return 1;
    }
    int returnCode = ReadWriteWithDecompress(InputFile, OutputFile);
    fclose(InputFile);
    fclose(OutputFile);
    return returnCode;
}

int main(const int argc, const char* argv[])
{
    if (argc < 4)
    {
        perror("Ошибка: не указаны все аргументы, ""rle -compress <input-file-name> <output-file-name>""");
        return 1;
    }
    if (strcmp(argv[1], "-compress") == 0)
    {
        return Compress(argv);
    }
    if (strcmp(argv[1], "-decompress") == 0)
    {
        return Decompress(argv);
    }
    perror("Ошибка: неверный первый аргумент");
    return 1;
}
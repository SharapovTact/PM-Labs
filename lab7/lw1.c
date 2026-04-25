#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my-str.c"



int WriteCompressedBytes(FILE* OutputFile, int byteCount, const int ch)
{
    while (byteCount >= 255)
    {
        fputc(255, OutputFile);
        fputc(ch, OutputFile);
        byteCount -= 255;
    }
    if (byteCount > 0)
    {
        fputc(byteCount, OutputFile);
        fputc(ch, OutputFile);
    }
    return 0;
}

int ReadWriteWithCompress(FILE* InputFile, FILE* OutputFile)
{
    int ch;
    if ((ch = fgetc(InputFile)) == EOF) {
        return 0;
    }
    //TODO завести структуру хранящую состояние компрессора
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

int Compress(const char* inp, const char* out) //TODO принять нормальное кло-во аргументов
{
    //TODO с маленькой буквы
    FILE* inputFile = fopen(inp, "rb");
    if (inputFile == NULL)
    {
        perror("Ошибка открытия файла");
        return 1;
    }
    FILE* outputFile = fopen(out, "wb");
    if (outputFile == NULL)
    {
        //TODO закрыть оба файла
        perror("Ошибка открытия файла");
        return 1;
    }
    int returnCode = ReadWriteWithCompress(inputFile, outputFile);
    fclose(inputFile);
    fclose(outputFile);
    return returnCode;
}

int ReadWriteWithDecompress(FILE* InputFile, FILE* OutputFile)
{
    int chCount;
    while ((chCount = fgetc(InputFile)) != EOF)
    {
        const int ch = fgetc(InputFile);
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

int Decompress(const int argc, const char* argv[])
{
    FILE* inputFile = fopen(argv[2], "rb");
    if (inputFile == NULL)
    {
        perror("Ошибка открытия файла");
        return 1;
    }
    FILE* outputFile = fopen(argv[3], "wb");
    if (outputFile == NULL)
    {
        perror("Ошибка открытия файла");
        return 1;
    }
    int returnCode = ReadWriteWithDecompress(inputFile, outputFile);
    fclose(inputFile);
    fclose(outputFile);
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
        return Compress(argv[2], argv[3]);
    }
    if (strcmp(argv[1], "-decompress") == 0)
    {
        return Decompress(argv[1], "-compress");
    }
    perror("Ошибка: неверный первый аргумент");
    return 1;
}
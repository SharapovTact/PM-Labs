#include <stdio.h>
#include <stdlib.h>
#include <string.h>

constexpr int DEFAULT_STR_LEN = 4;

char *ReadLine()
{
    char *s = NULL;
    size_t len = 0;
    int chCode;
    int size = DEFAULT_STR_LEN;
    char *tmp = malloc(size);
    if (tmp == NULL)
    {
        goto error;
    }
    while ((chCode = getchar()) != '\n' && chCode != EOF)
    {
        if (len == size)
        {
            size *= 2;
            tmp = realloc(s, size);
        }
        if (tmp == NULL)
        {
            goto error;
        }
        s = tmp;
        s[len++] = (char)chCode;
    }
    tmp = realloc(s, len + 1);
    if (tmp == NULL)
    {
        goto error;
    }
    s = tmp;
    s[len] = '\0';
    return s;
    error:
        if (s != NULL)
        {
            free(s);
        }
    return NULL;
}
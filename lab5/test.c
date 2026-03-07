#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

enum State
{
    OK = 0,
    NOT_FOUND = 404,
    ERROR = 500,
};
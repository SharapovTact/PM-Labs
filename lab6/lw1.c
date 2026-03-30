#include <errno.h>
#include <inttypes.h>

#include "my-dump.c"

void PutSign(unsigned char *p, const bool sign)
{
    constexpr int SIGN_BYTE = 3;
    constexpr int SEVEN_BIT = 7;
    p[SIGN_BYTE] = (unsigned char)sign << SEVEN_BIT;
}

// Я не хочу юзать memcpy, там получается вот так:
/*void PutSign(unsigned char *p, const bool sign)
{
    constexpr int SIGN_BYTE = 3;
    constexpr int SEVEN_BIT = 7;
    unsigned char value = (unsigned char)sign << SEVEN_BIT;
    memcpy(p + SIGN_BYTE, &value, 1);
}*/

void PutExponent(unsigned char *p, const uint8_t exponent)
{
    constexpr int EXP_BYTE_FIRST = 3;
    constexpr int EXP_BYTE_SECOND = 2;
    p[EXP_BYTE_FIRST] = p[EXP_BYTE_FIRST] | exponent >> 1;
    p[EXP_BYTE_SECOND] = exponent << 7;
}

void PutMantissa(unsigned char *p, const uint32_t mantissa)
{
    constexpr int MAX_MANTISSA_BYTE = 2;
    p[MAX_MANTISSA_BYTE] = p[MAX_MANTISSA_BYTE] | ((mantissa >> 16) & 0x7F);
    p[MAX_MANTISSA_BYTE - 1] = p[MAX_MANTISSA_BYTE - 1] | mantissa << 16 >> 24;
    p[MAX_MANTISSA_BYTE - 2] = p[MAX_MANTISSA_BYTE - 2] | mantissa << 24 >> 24;
}

int BuildFloat(const bool sign, const uint8_t exponent, const uint32_t mantissa, float* result)
{
    *result = 0;
    unsigned char *p = (unsigned char *)result;
    PutSign(p, sign);
    PutExponent(p, exponent);
    PutMantissa(p, mantissa);
    return 0;
}

int InputFloat(bool *sign, uint8_t *exponent, uint32_t *mantissa)
{
    constexpr int MANTISSA_MAX = 0b00000000011111111111111111111111;
    constexpr int EXPONENT_MAX = 255;
    printf("Введите знак: ");
    if (scanf("%d", sign) != 1) //TODO добавить проверку на диапазон
    {
        return EBADMSG;
    }
    printf("Введите экспоненту: ");
    if (scanf("%" SCNu8, exponent) != 1)
    {
        return EBADMSG;
    }
    printf("Введите мантиссу: ");
    if (scanf("%" SCNu32, mantissa) != 1)
    {
        return EBADMSG;
    }
    if (*exponent > EXPONENT_MAX || *mantissa > MANTISSA_MAX )
    {
        return EBADMSG;
    }
    return 0;
}

int main(void)
{
    /*0,255,0
    1,255,0
    1,127,0
    0,129,2097152
    1,125,0
    0,0,0*/
    bool sign;
    uint8_t exponent;
    uint32_t mantissa;
    float num = 1.5;
    PrintDump((uint8_t *)&num, sizeof(float));
    if (InputFloat(&sign, &exponent, &mantissa) != 0)
    {
        return EBADMSG;
    }
    BuildFloat(sign, exponent, mantissa, &num);
    PrintDump((uint8_t *)&num, sizeof(float));
    printf("%f", num);
}
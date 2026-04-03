#include <errno.h>
#include <inttypes.h>
#include <string.h>

#include "my-dump.c"

int BuildFloat(const bool sign, const uint8_t exponent, const uint32_t mantissa, float* result) //TODO сперва сделать uint32, полностью собрать и memcpy
{
    uint32_t buildingNum = 0;
    buildingNum = sign << 31;
    buildingNum = buildingNum | exponent << 23;
    buildingNum = buildingNum | mantissa;
    memcpy((uint8_t *)result, (uint8_t *)&buildingNum, sizeof(float));
    return 0;
}

int InputFloat(bool *sign, uint8_t *exponent, uint32_t *mantissa)
{
    constexpr int MANTISSA_MAX = 0x7FFFFF;
    constexpr int EXPONENT_MAX = 255;
    printf("Введите знак: ");
    if (scanf("%d", sign) != 1)
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

int main(void) //TODO разобраться как представить число 0,75 в этой системе
{
    bool sign;
    uint8_t exponent;
    uint32_t mantissa;
    float num = 0;
    if (InputFloat(&sign, &exponent, &mantissa) != 0)
    {
        return EBADMSG;
    }
    BuildFloat(sign, exponent, mantissa, &num);
    PrintDump((uint8_t *)&num, sizeof(float));
    printf("%f", num);
}
#include "lib/string.h"
#include "core/common.h"

void _itoa_swap(char *x, char *y) { char t = *x; *x = *y; *y = t; }

char* _itoa_rev(char *buffer, int i, int j)
{
    while (i < j) { _itoa_swap(&buffer[i++], &buffer[j--]); }
    return buffer;
}

void _ultoa(unsigned long value, char* result, int base)
{
    unsigned char index;
    char buffer[32];
    index = 32;
    do 
    {
        buffer[--index] = '0' + (value % base);
        if ( buffer[index] > '9') { buffer[index] += 'A' - ':'; }
        value /= base;
    } while (value != 0);

    do { *result++ = buffer[index++]; } while (index < 32);
    *result = 0;
}

void _ftoa_rev(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) 
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int _ftoa_conv(int x, char str[], int d)
{
    int i = 0;
    while (x) { str[i++] = (x % 10) + '0'; x = x / 10; }

    while (i < d) { str[i++] = '0'; }

    _ftoa_rev(str, i);
    str[i] = '\0';
    return i;
}

// ----------------------------------------------------------------------------------------------------------------------------------------

char* OX_CreateString(const char* str)
{
    if (str == NULL) { return NULL; }

    size_t len = strlen(str);
    if (len == 0) { return NULL; }

    char* out = OX_Allocate(len + 1);
    OX_MemoryCopy(out, str, len);
    return out;
}

char* OX_CreateStringEx(const char* str, size_t sz)
{
    if (str == NULL) { return NULL; }

    size_t len = strlen(str);
    if (len == 0) { return OX_Allocate(sz); }
    if (sz < len + 1) { OX_DebugError("OX_CreateStringEx(%p, %lu) - Buffer size too small", str, sz); return NULL; }

    char* out = OX_Allocate(sz);
    OX_MemoryCopy(out, str, len);
    return out;
}

void OX_FreeString(char* str)
{
    if (str == NULL) { OX_DebugError("OX_FreeString(%p) - Null pointer", str); return; }
    OX_Free(str);
}

bool OX_StringEquals(const char* str1, const char* str2) 
{ 
    if (str1 == NULL || str2 == NULL) { return false; }
    return (strcmp(str1, str2) == 0 ? true : false);
}

bool OX_StringEqualsEx(const char* str1, const char* str2, size_t sz)
{
    if (str1 == NULL || str2 == NULL || sz == 0) { return false; }
    return (strcmp(str1, str2) == 0 ? true : false);
}

int OX_StringComp(const char* str1, const char* str2)
{
    if (str1 == NULL || str2 == NULL) { return false; }
    return strcmp(str1, str2);
}

int OX_StringCompEx(const char* str1, const char* str2, size_t sz)
{
    if (str1 == NULL || str2 == NULL || sz == 0) { return false; }
    return strncmp(str1, str2, sz);
}

bool OX_IsDecimal(const char* str)
{
    if (str == NULL || strlen(str) == 0) { return false; }

    int i = 0;
    while (str[i] != 0) { if (!isdigit(str[i++])) { return false; } }
    return true;
}

bool OX_IsHex(const char* str)
{
    if (str == NULL || strlen(str) == 0) { return false; }

    int i = 0;
    while (str[i] != 0) { if (!isxdigit(str[i++])) { return false; } }
    return true;
}

bool OX_IsFloat(const char* str)
{
    if (str == NULL || strlen(str) == 0) { return false; }

    int dots = 0, len = (int)strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (str[i] == '.') { dots++; }
        if (!isdigit(str[i]) && str[i] != '.') { return false; }
        if (dots > 1) { return false; }
    }
    return (dots == 0 || dots == 1);
}

char* OX_FloatToString(float num, char* buff, int dot)
{
    int   ipart = (int)num;
    float fpart = num - (float)ipart;
    int   i     = _ftoa_conv(ipart, buff, 0);

    if (dot != 0) 
    {
        buff[i] = '.';
        fpart = fpart * powf(10, dot);
        _ftoa_conv((int)fpart, buff + i + 1, dot);
    }
    return buff;
}

float OX_StringToFloat(const char* str)
{
    double a;           /* the a value in a*10^b */
    double decplace;    /* number to divide by if decimal point is seen */
    double b;           /* The b value (exponent) in a*10^b */

    int sign = 1;       /* stores the sign of a */
    int bsign = 1;      /* stores the sign of b */

    while (*str && isspace(*str))
        ++str;

    if (*str == '+')
        ++str;
    if (*str == '-') {
        sign = -1;
        ++str;
    }
    if ((*str == 'n' || *str == 'N') && 
    (str[1] == 'a' || str[1] == 'A') 
    && (str[2] == 'n' || str[2] == 'N'))
            return NAN * sign;
    if ((*str == 'i' || *str == 'I') && (str[1] == 'n' || str[1] == 'N') && 
        (str[2] == 'f' || str[2] == 'F'))
            return INFINITY * sign;

    for (a = 0; *str && isdigit(*str); ++str)
        a = a * 10 + (*str - '0');

    if (*str == '.')
        ++str;
    for (decplace = 1.; *str && isdigit(*str); ++str, decplace *= 10.)
        a = a * 10. + (*str - '0');

    if (*str == 'e' || *str == 'E') {
        /* if the user types a string starting from e, make the base be 1 */
        if (a == 0)
            a = 1;
        ++str;
        if (*str == '-') {
            bsign = -1;
            ++str;
        }
        if (*str == '+')
            ++str;
        for (b = 0; *str && isdigit(*str); ++str)
            b = b * 10 + (*str - '0');

        b *= bsign;
    }
    else
        b = 0;

    return (a * sign / decplace) * pow(10, b);
}

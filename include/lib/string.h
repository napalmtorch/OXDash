#pragma once
#include "lib/types.h"

char* OX_CreateString(const char* str);
char* OX_CreateStringEx(const char* str, size_t sz);
void  OX_FreeString(char* str);

bool OX_StringEquals(const char* str1, const char* str2);
bool OX_StringEqualsEx(const char* str1, const char* str2, size_t sz);
int  OX_StringComp(const char* str1, const char* str2);
int  OX_StringCompEx(const char* str1, const char* str2, size_t sz);

bool OX_IsDecimal(const char* str);
bool OX_IsHex(const char* str);
bool OX_IsFloat(const char* str);

char* OX_FloatToString(float num, char* buff, int dot);
float OX_StringToFloat(const char* str);

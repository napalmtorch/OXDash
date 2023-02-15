#pragma once
#include "lib/types.h"

typedef struct OX_INIFile OX_INIFile;
typedef struct OX_INIValue OX_INIValue;
typedef struct OX_INISection OX_INISection;
typedef struct OX_INIToken OX_INIToken;

typedef enum
{
    OX_INITYPE_INVALID,
    OX_INITYPE_NUM,
    OX_INITYPE_STR,
    OX_INITYPE_CHAR,
    OX_INITYPE_BOOL,
} OX_INITYPE;

typedef enum
{
  OX_INITOK_INVALID,
  OX_INITOK_NUM,
  OX_INITOK_STR,
  OX_INITOK_CHAR,
  OX_INITOK_BOOL,
  OX_INITOK_ID,
  OX_INITOK_EQL,
  OX_INITOK_BEGIN,
  OX_INITOK_END,
  OX_INITOK_NL,  
} OX_INITOK;

struct OX_INIToken
{
      OX_INITOK type;
      int       line;
      char*     value;
};

struct OX_INIValue
{
    OX_INITYPE     type;
    OX_INISection* section;
    char*          name;
    void*          data;
    size_t         sz;
};

struct OX_INISection
{
    char*        name;
    OX_INIValue* values;
    size_t       values_count;
};

struct OX_INIFile
{
    char*          input;
    OX_INIToken*   tokens;
    OX_INISection* sections;
    char*          word;
    size_t         wordsz;
    size_t         input_count;
    size_t         tok_count;
    size_t         sect_count;
    int            line, pos;
};


OX_INIFile OX_LoadINI(const char* path);
void OX_FreeINI(OX_INIFile* file);

void* OX_ParseValueINI(OX_INIFile* file, OX_INITOK type, size_t sz, const char* valstr);

OX_INISection* OX_GetINISection(OX_INIFile* file, const char* name);
OX_INIValue*   OX_GetINIValue(OX_INIFile* file, OX_INISection* section, const char* name);

int      OX_ReadIntINI(OX_INIFile* file, const char* section, const char* name);
uint32_t OX_ReadUIntINI(OX_INIFile* file, const char* section, const char* name);
float    OX_ReadFloatINI(OX_INIFile* file, const char* section, const char* name);
char*    OX_ReadStringINI(OX_INIFile* file, const char* section, const char* name);
char     OX_ReadCharINI(OX_INIFile* file, const char* section, const char* name);
bool     OX_ReadBoolINI(OX_INIFile* file, const char* section, const char* name);
bool     OX_IsValidINI(OX_INIFile* file);
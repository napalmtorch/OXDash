#include "lib/parsing/ini.h"
#include "core/common.h"

void OX_ParseINI(OX_INIFile* file);
void OX_ParseEqlINI(OX_INIFile* file);
void OX_ParseBeginINI(OX_INIFile* file);
void OX_ParseEndINI(OX_INIFile* file);
void OX_ParseQuotesINI(OX_INIFile* file, bool str);
void OX_ParseCommentINI(OX_INIFile* file);
void OX_ParseWordINI(OX_INIFile* file);
void OX_ParseNewlineINI(OX_INIFile* file);

OX_INIFile OX_LoadINI(const char* path)
{
    OX_INIFile file;
    file.input      = (char*)OX_ReadFile(path, &file.input_count);
    file.tokens     = NULL;
    file.sections   = NULL;
    file.pos        = 0;
    file.line       = 0;
    file.word       = 0;
    file.wordsz     = 0;
    file.sect_count = 0;
    file.tok_count  = 0;
    OX_ParseINI(&file);
    return file;
}

void OX_FreeINI(OX_INIFile* file)
{
    if (file == NULL)        { OX_DebugError("OX_FreeINI(00000000) - Null pointer"); return; }

    if (file->sections != NULL)
    {     
        size_t i, j;
        for (i = 0; i < file->sect_count; i++) 
        { 
            OX_INISection* sect = &file->sections[i];

            if (sect->values != NULL) 
            { 
                for (j = 0; j < sect->values_count; j++)
                {
                    if (sect->values[j].data != NULL) 
                    { 
                        OX_Free(sect->values[j].data); 
                        sect->values[j].data = NULL; 
                    }
                    if (file->sections[i].values[j].name != NULL) { OX_FreeString(file->sections[i].values[j].name); file->sections[i].values[j].name = NULL; }
                }
                OX_Free(file->sections[i].values);
            } 
        }
        OX_Free(file->sections);
    }

    if (file->input != NULL) { OX_FreeString(file->input); file->input = NULL; }
    if (file->word != NULL)  { OX_FreeString(file->word); file->word = NULL; }
    if (file->tokens != NULL)
    {
        for (size_t i = 0; i < file->tok_count; i++) 
        {
            if (file->tokens[i].value != NULL) { OX_FreeString(file->tokens[i].value); }
        }
        OX_Free(file->tokens);
    }
}

void* OX_ParseValueINI(OX_INIFile* file, OX_INITOK type, size_t sz, const char* valstr)
{
    void* data = OX_Allocate(sz);
    OX_MemoryFill(data, 0, sz);
    if (type == OX_INITOK_NUM)
    {
        if (OX_IsDecimal(valstr))
        {
            int num = atoi(valstr);
            ((int*)data)[0] = num;
        }
        else if (OX_IsHex(valstr))
        {
            uint32_t num = strtol(valstr, NULL, 16);
            ((uint32_t*)data)[0] = num;
        }
        else if (OX_IsFloat(valstr))
        {
            float num = (float)OX_StringToFloat(valstr);
            ((float*)data)[0] = num;
        }
        return data;
    }
    else if (type == OX_INITOK_BOOL)
    {
        if (!strcmp(valstr, "true")) { ((uint8_t*)data)[0] = true; }
        else if (!strcmp(valstr, "false")) { ((uint8_t*)data)[0] = false; }
        return data;
    }
    else if (type == OX_INITOK_CHAR) { ((char*)data)[0] = valstr[0]; return data; }
    else if (type == OX_INITOK_STR) { strcpy((char*)data, valstr); return data; }
    OX_DebugError("OX_ParseValueINI(%p, %02X, %u, %s) - Parsing error", file, (uint8_t)type, sz, valstr);
    return NULL;
}

OX_INISection* OX_GetINISection(OX_INIFile* file, const char* name)
{
    if (file == NULL) { OX_DebugError("OX_GetINISection(%p, %p) - Null pointer", file, name); return NULL; }
    if (name == NULL || strlen(name) == 0) { OX_DebugError("OX_GetINISection(%p, %p) - Invalid name", file, name); return NULL; }

    for (size_t i = 0; i < file->sect_count; i++)
    {
        if (OX_StringEquals(file->sections[i].name, name)) { return &file->sections[i]; }
    }
    return NULL;
}

OX_INIValue* OX_GetINIValue(OX_INIFile* file, OX_INISection* section, const char* name)
{
    if (file == NULL || section == NULL) { OX_DebugError("OX_GetINIValue(%p, %p, %p) - Null pointer", file, section, name); return NULL; }
    if (name == NULL || strlen(name) == 0) { OX_DebugError("OX_GetINIValue(%p, %p, %p) - Invalid name", file, section, name); return NULL; }

    for (size_t i = 0; i < section->values_count; i++)
    {
        if (OX_StringEquals(section->values[i].name, name)) { return &section->values[i]; }
    }
    return NULL;
}

int OX_ReadIntINI(OX_INIFile* file, const char* section, const char* name)
{
    OX_INISection* sect = OX_GetINISection(file, section);
    if (sect == NULL) { OX_DebugError("OX_ReadIntINI(%p, %s, %s) - No section with name", file, section, name); return 0; }
    OX_INIValue* val = OX_GetINIValue(file, sect, name);
    if (val == NULL) { OX_DebugError("OX_ReadIntINI(%p, %s, %s) - No value with name in section", file, section, name); return 0; }
    return ((int*)val->data)[0];
}

uint32_t OX_ReadUIntINI(OX_INIFile* file, const char* section, const char* name)
{
    OX_INISection* sect = OX_GetINISection(file, section);
    if (sect == NULL) { OX_DebugError("OX_ReadUIntINI(%p, %s, %s) - No section with name", file, section, name); return 0; }
    OX_INIValue* val = OX_GetINIValue(file, sect, name);
    if (val == NULL) { OX_DebugError("OX_ReadUIntINI(%p, %s, %s) - No value with name in section", file, section, name); return 0; }
    return ((uint32_t*)val->data)[0];
}

float OX_ReadFloatINI(OX_INIFile* file, const char* section, const char* name)
{
    OX_INISection* sect = OX_GetINISection(file, section);
    if (sect == NULL) { OX_DebugError("OX_ReadFloatINI(%p, %s, %s) - No section with name", file, section, name); return 0; }
    OX_INIValue* val = OX_GetINIValue(file, sect, name);
    if (val == NULL) { OX_DebugError("OX_ReadFloatINI(%p, %s, %s) - No value with name in section", file, section, name); return 0; }
    return ((float*)val->data)[0];
}

char* OX_ReadStringINI(OX_INIFile* file, const char* section, const char* name)
{
    OX_INISection* sect = OX_GetINISection(file, section);
    if (sect == NULL) { OX_DebugError("OX_ReadStringINI(%p, %s, %s) - No section with name", file, section, name); return 0; }
    OX_INIValue* val = OX_GetINIValue(file, sect, name);
    if (val == NULL) { OX_DebugError("OX_ReadStringINI(%p, %s, %s) - No value with name in section", file, section, name); return 0; }
    return (char*)val->data;
}

char OX_ReadCharINI(OX_INIFile* file, const char* section, const char* name)
{
    OX_INISection* sect = OX_GetINISection(file, section);
    if (sect == NULL) { OX_DebugError("OX_ReadCharINI(%p, %s, %s) - No section with name", file, section, name); return 0; }
    OX_INIValue* val = OX_GetINIValue(file, sect, name);
    if (val == NULL) { OX_DebugError("OX_ReadCharINI(%p, %s, %s) - No value with name in section", file, section, name); return 0; }
    return ((char*)val->data)[0];
}

bool OX_ReadBoolINI(OX_INIFile* file, const char* section, const char* name)
{
    OX_INISection* sect = OX_GetINISection(file, section);
    if (sect == NULL) { OX_DebugError("OX_ReadBoolINI(%p, %s, %s) - No section with name", file, section, name); return 0; }
    OX_INIValue* val = OX_GetINIValue(file, sect, name);
    if (val == NULL) { OX_DebugError("OX_ReadBoolINI(%p, %s, %s) - No value with name in section", file, section, name); return 0; }
    return ((bool*)val->data)[0];
}

bool OX_IsValidINI(OX_INIFile* file)
{
    if (file == NULL) { OX_DebugError("OX_IsValidINI(00000000) - Null pointer"); return false; }
    if (file->sect_count == 0 || file->sections == NULL) { return false; }
    return true;
}

void OX_AddTokenINI(OX_INIFile* file, OX_INIToken tok)
{
    if (file == NULL) { OX_DebugError("OX_AddTokenINI(%p, %p) - Null pointer", file, &tok); return; }

    OX_INIToken* toks = (OX_INIToken*)OX_Allocate((file->tok_count + 1) * sizeof(OX_INIToken));
    if (file->tokens != NULL) { OX_MemoryCopy(toks, file->tokens, file->tok_count * sizeof(OX_INIToken)); OX_Free(file->tokens); }

    file->tokens = toks;
    file->tokens[file->tok_count] = tok;
    file->tok_count++;
}

void OX_ParseINI(OX_INIFile* file)
{
    if (file == NULL) { OX_DebugError("OX_ParseINI(00000000) - Null pointer"); return; }

    file->wordsz = file->input_count + 1;
    file->word   = (char*)OX_Allocate(file->wordsz);
    file->line   = 1;
    file->pos    = 0;
    
    while (file->pos < file->input_count)
    {
        char c = file->input[file->pos++];

        switch (c)
        {
            case '\r': { break; }
            case '\t': { break; }
            case '\b': { break; }
            case '\n': { OX_ParseNewlineINI(file); break; }
            case ' ':  { OX_ParseWordINI(file); break; }
            case '\'': { OX_ParseQuotesINI(file, false); break; }
            case '\"': { OX_ParseQuotesINI(file, true); break; }
            case '=':  { OX_ParseEqlINI(file); break; }
            case '[':  { OX_ParseBeginINI(file); break; }
            case ']':  { OX_ParseEndINI(file); break; }
            default:
            {
                if (isprint(c)) { strncat(file->word, &c, 1); }
                break;
            }
        }        
    }
    OX_ParseWordINI(file);
    OX_FreeString(file->word);

    file->word = NULL;
    file->pos = 0;

    while (file->pos < file->tok_count)
    {
        OX_INIToken tok = file->tokens[file->pos++];
        if (tok.type == OX_INITOK_NL) { continue; }
        
        if (tok.type == OX_INITOK_BEGIN)
        {
            OX_INIToken tok_name = file->tokens[file->pos++];
            if (tok_name.type != OX_INITOK_ID) { OX_DebugError("OX_ParseINI(%p) - Expected identifier at line %d", file, tok_name.line); return; }
            OX_INIToken tok_end = file->tokens[file->pos++];
            if (tok_end.type != OX_INITOK_END) { OX_DebugError("OX_ParseINI(%p) - Expected ']' at line %d", file, tok_end.line); return; }
            OX_INISection sect = (OX_INISection){ tok_name.value, NULL, 0 };

            OX_INISection* sections = (OX_INISection*)OX_Allocate((file->sect_count + 1) * sizeof(OX_INISection));
            if (file->sections != NULL) { OX_MemoryCopy(sections, file->sections, file->sect_count * sizeof(OX_INISection)); OX_Free(file->sections); }

            file->sections = sections;
            file->sections[file->sect_count] = sect;
            file->sect_count++;
        }
        else if (tok.type == OX_INITOK_ID)
        {   
            OX_INIToken tok_eql = file->tokens[file->pos++];
            if (tok_eql.type != OX_INITOK_EQL) { OX_DebugError("OX_ParseINI(%p) - Expected '=' at line %d - %s", file, tok_eql.line, tok_eql.value); return; }
            OX_INIToken tok_val = file->tokens[file->pos++];
            if (tok_val.type == OX_INITOK_INVALID || tok_val.type > OX_INITOK_BOOL) { OX_DebugError("OX_ParseINI(%p) - Invalid value at line %d", file, tok_val.line); return; }

            size_t sz = 0;
            if (tok_val.type == OX_INITOK_NUM) { sz = 4; }
            else if (tok_val.type == OX_INITOK_BOOL || tok_val.type == OX_INITOK_CHAR) { sz = 1; }
            else if (tok_val.type == OX_INITOK_STR) { sz = strlen(tok_val.value) + 1; }
            void* data = OX_ParseValueINI(file, tok_val.type, sz, tok_val.value);

            OX_INISection* sect   = &file->sections[file->sect_count - 1];
            OX_INIValue    value  = (OX_INIValue){ (OX_INITYPE)tok_val.type, sect, OX_CreateString(tok.value), data, sz,  }; 
            OX_INIValue*   values = (OX_INIValue*)OX_Allocate((sect->values_count + 1) * sizeof(OX_INIValue));
            
            if (sect->values != NULL) { OX_MemoryCopy(values, sect->values, sect->values_count * sizeof(OX_INIValue)); OX_Free(sect->values); }

            sect->values = values;
            sect->values[sect->values_count] = value;
            sect->values_count++;
        }
    }
}

void OX_ParseEqlINI(OX_INIFile* file)
{
    OX_ParseWordINI(file);
    char* str = OX_CreateString("=");
    OX_INIToken tok = (OX_INIToken){ OX_INITOK_EQL, file->line, str };
    OX_AddTokenINI(file, tok);
}

void OX_ParseBeginINI(OX_INIFile* file)
{
    OX_ParseWordINI(file);
    char* str = OX_CreateString("[");
    OX_INIToken tok = (OX_INIToken){ OX_INITOK_BEGIN, file->line, str };
    OX_AddTokenINI(file, tok);
}

void OX_ParseEndINI(OX_INIFile* file)
{
    OX_ParseWordINI(file);
    char* str = OX_CreateString("]");
    OX_INIToken tok = (OX_INIToken){ OX_INITOK_END, file->line, str };
    OX_AddTokenINI(file, tok);
}

void OX_ParseQuotesINI(OX_INIFile* file, bool str)
{
    OX_ParseWordINI(file);
    char* temp = (char*)OX_Allocate(file->input_count + 1);
    while (file->pos < file->input_count)
    {
        char c = file->input[file->pos++];
        if (c == '\n') { OX_DebugError("OX_ParseQuotesINI(%p, %d) - Unterminated value", file, str); return; }
        if (c == '\'' && !str) { break; }
        if (c == '\"' && str) { break; }
        strncat(temp, &c, 1);
    }
    char* value = OX_CreateString(temp);
    OX_INIToken tok = (OX_INIToken){ (str ? OX_INITOK_STR : OX_INITOK_CHAR), file->line, value };
    OX_AddTokenINI(file, tok);
    OX_FreeString(temp);
}

void OX_ParseCommentINI(OX_INIFile* file)
{
    OX_ParseWordINI(file);
    while (file->pos < file->input_count)
    {
        char c = file->input[file->pos++];
        if (c == '\n' || c == 0) { break; }
    }
}

void OX_ParseWordINI(OX_INIFile* file)
{
    if (file == NULL) { OX_DebugError("OX_ParseWordINI(00000000) - Null pointer"); return; }
    if (file->word == NULL || strlen(file->word) == 0 || OX_StringEquals(file->word, " ")) { OX_MemoryFill(file->word, 0, file->wordsz); return; }

    OX_INITOK type = OX_INITOK_ID;

    if (OX_StringEquals(file->word, "true") || OX_StringEquals(file->word, "false")) { type = OX_INITOK_BOOL; }
    else if (OX_IsDecimal(file->word)) { type = OX_INITOK_NUM; }
    else if (file->word[0] == '0' && toupper(file->word[1]) == 'X' && OX_IsHex(file->word + 2))
    {
        type = OX_INITOK_NUM;
        char* temp = OX_CreateString(file->word + 2);
        OX_MemoryFill(file->word, 0, file->wordsz);
        strcpy(file->word, temp);
        OX_FreeString(temp);
    }
    else if (OX_IsFloat(file->word)) { type = OX_INITOK_NUM; }

    char* tok_val = OX_CreateString(file->word);
    OX_INIToken tok = (OX_INIToken){ type, file->line, tok_val };
    OX_AddTokenINI(file, tok);

    OX_MemoryFill(file->word, 0, file->wordsz);
}

void OX_ParseNewlineINI(OX_INIFile* file)
{
    OX_ParseWordINI(file);
    char* str = OX_CreateString("NL");
    OX_INIToken tok = (OX_INIToken){ OX_INITOK_NL, file->line++, str };
    OX_AddTokenINI(file, tok);
}
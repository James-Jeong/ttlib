#ifndef __STRLIB_H__
#define __STRLIB_H__

// 문자열 관리 구조체
typedef struct _string_t {
    size_t length;
    char *data;
} String, *StringPtr, **StringPtrContainer;

///////////////////////////////////////////////////////////////////////////////
// 공용 APIs
StringPtr NewString(char *s);
void DeleteString(StringPtrContainer pString);
StringPtr CloneString(StringPtr str);

size_t GetLength(StringPtr str);
char* GetPtr(StringPtr str);

char* SetString(StringPtr str, char *s);
char* ConvertToUpperCase(StringPtr str);

#endif

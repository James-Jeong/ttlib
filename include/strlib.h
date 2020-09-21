#ifndef __STRLIB_H__
#define __STRLIB_H__

///////////////////////////////////////////////////////////////////////////////
/// Definition
///////////////////////////////////////////////////////////////////////////////

// 문자열 관리 구조체
typedef struct _string_t {
    size_t length;
    char *data;
} String, *StringPtr, **StringPtrContainer;

///////////////////////////////////////////////////////////////////////////////
// 공용 APIs
///////////////////////////////////////////////////////////////////////////////

StringPtr NewString(const char *s);
void DeleteString(StringPtrContainer pString);
StringPtr CloneString(const StringPtr str);

size_t GetLength(const StringPtr str);
char* GetPtr(const StringPtr str);

char* SetString(StringPtr str, const char *s);
char* ConvertToUpperCase(StringPtr str);
char* ConvertToLowerCase(StringPtr str);

char* RemoveLeftSpace(StringPtr str);
char* RemoveRightSpace(StringPtr str);
char* RemoveBothSpace(StringPtr str);

char* IntactCopy(StringPtr dstStr, const StringPtr srcStr);
char* RestrictedCopy(StringPtr dstStr, const StringPtr srcStr, size_t length);

#endif

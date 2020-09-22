#ifndef __STRLIB_H__
#define __STRLIB_H__

///////////////////////////////////////////////////////////////////////////////
/// Definition
///////////////////////////////////////////////////////////////////////////////

// vsnprintf 동작 성공
#ifndef VSN_FAIL
#define VSN_FAIL -1
#endif

// 문자열 관리 구조체
typedef struct _string_t {
    int length;
    char *data;
} String, *StringPtr, **StringPtrContainer;

///////////////////////////////////////////////////////////////////////////////
// 공용 APIs
///////////////////////////////////////////////////////////////////////////////

StringPtr NewString(const char *s);
void DeleteString(StringPtrContainer pString);
StringPtr CloneString(const StringPtr str);

int GetLength(const StringPtr str);
char* GetPtr(const StringPtr str);

char* SetString(StringPtr str, const char *s);
char* ConvertToUpperCase(StringPtr str);
char* ConvertToLowerCase(StringPtr str);

char* RemoveLeftSpace(StringPtr str);
char* RemoveRightSpace(StringPtr str);
char* RemoveBothSpace(StringPtr str);

char* CopyString(StringPtr dstStr, const StringPtr srcStr);
char* CopyNString(StringPtr dstStr, const StringPtr srcStr, int length);

char* FormatString(StringPtr str, const char* format, ...);

char* ConcatString(StringPtr str, const char* s);
char* TruncateString(StringPtr str, int from);
StringPtr SubString(StringPtr str, int from, int length);

#endif

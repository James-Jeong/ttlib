#ifndef __STRLIB_H__
#define __STRLIB_H__

///////////////////////////////////////////////////////////////////////////////
/// Enums
///////////////////////////////////////////////////////////////////////////////

// 공통 Bool 열거형
typedef enum Boolean
{
	// 조건 거짓
	False = -1,
	// 조건 참
	True = 1
} Bool;

// SplitString 함수 옵션 열거형
typedef enum SplitOption
{
	// 빈문자열 포함
	IncludeEmptyArray = 0,
	// 빈문자열 제외
	ExcludeEmptyArray = 1
} SplitOption;

///////////////////////////////////////////////////////////////////////////////
/// Macros
///////////////////////////////////////////////////////////////////////////////

// vsnprintf 동작 성공
#ifndef VSN_FAIL
#define VSN_FAIL -1
#endif

// CompareString 함수 동작 오류
#ifndef COMPARE_ERROR
#define COMPARE_ERROR -2
#endif

// SearchString 함수 동작 오류
#ifndef SEARCH_ERROR
#define SEARCH_ERROR -2
#endif

///////////////////////////////////////////////////////////////////////////////
/// Definition
///////////////////////////////////////////////////////////////////////////////

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
StringPtr SubString(const StringPtr str, int from, int length);

int CompareString(const StringPtr str1, const StringPtr str2);
Bool SearchString(const StringPtr str, const char *pattern);

Bool IsDigit(char c);
Bool IsAlpha(char c);
Bool IsLetter(char c);
Bool IsSpace(char c);
Bool IsCRLF(char c);
char** SplitString(const char *s, char delimiter, SplitOption option);
char* MergeString(char **sList, char delimiter);

#endif

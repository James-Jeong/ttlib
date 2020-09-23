#ifndef __STRLIB_H__
#define __STRLIB_H__

///////////////////////////////////////////////////////////////////////////////
/// Enums
///////////////////////////////////////////////////////////////////////////////

typedef enum SearchResult
{
	// 검색 실패
	SearchFalse = -1,
	// 검색 성공
	SearchTrue = 1
} SearchResult;

typedef enum CharConditionResult
{
	// 문자 검사 조건 거짓
	CFalse = -1,
	// 문자 검사 조건 참
	CTrue = 1
} CBool;

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
SearchResult SearchString(const StringPtr str, const char *pattern);

CBool IsDigit(char c);
CBool IsAlpha(char c);
CBool IsLetter(char c);
CBool IsSpace(char c);
CBool IsCRLF(char c);
char** SplitString(const char *s, char delimit);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "strlib.h"

////////////////////////////////////////////////////////////////////////////////
/// Static Definition
////////////////////////////////////////////////////////////////////////////////

// 문자와 관련된 작업을 하기 위한 함수 지정을 위한 함수 포인터
typedef int (*CharFunction_f)(int c);

////////////////////////////////////////////////////////////////////////////////
/// Predefinition of Static Functions
////////////////////////////////////////////////////////////////////////////////

static StringPtr CreateString();
static char* CloneCharArray(const char *s, int length);
static char** RemoveNullPointerInCharPtrContainer(char **container, int size);
static int* MakeDelimiterPosArray(const char *s, char delimiter, int *delimiterCount);
static char* ChangeStringCase(StringPtr str, CharFunction_f func);

////////////////////////////////////////////////////////////////////////////////
/// 공용 APIs
////////////////////////////////////////////////////////////////////////////////

/**
 * @fn StringPtr NewString(const char *s)
 * @brief 문자열 관리 구조체를 새로 생성하는 함수
 * @param s 구조체에서 관리할 문자열(입력, 읽기 전용)
 * @return 성공 시 새로 생성된 String 객체의 주소, 실패 시 NULL 반환
 */
StringPtr NewString(const char *s)
{
    if(s == NULL) return NULL;

    StringPtr newStr = CreateString();
    if(newStr == NULL)  return NULL;
    
	if(SetString(newStr, s) == NULL)
	{
		free(newStr);
		return NULL;
	}
	
    return newStr;
}

/**
 * @fn void DeleteString(StringPtrContainer stringPtrContainer)
 * @brief 문자열 관리 구조체를 삭제하는 함수
 * @param stringPtrContainer String 구조체의 포인터(주소)들을 관리하는 이중 포인터(입력)
 * @return 반환값 없음
 */
void DeleteString(StringPtrContainer stringPtrContainer)
{
	if((*stringPtrContainer)->data != NULL)
	{
	    free((*stringPtrContainer)->data);
	}

    free(*stringPtrContainer);
    *stringPtrContainer = NULL;
}

/**
 * @fn StringPtr CloneString(StringPtr str)
 * @brief 문자열 관리 구조체를 새로 복제하는 함수
 * @param str 복제할 문자열 관리 구조체(입력, 읽기 전용)
 * @return 성공 시 새로 복제된 String 객체 주소, 실패 시 NULL 또는 str 반환
 */
StringPtr CloneString(const StringPtr str)
{
    if(str == NULL || str->data == NULL || str->length <= 0) return NULL;
	StringPtr newStr = NewString(str->data);
    if(newStr == NULL) return str;
    return newStr;
}

/**
 * @fn int GetLength(const StringPtr str)
 * @brief 구조체에서 관리하는 문자열의 길이를 반환하는 함수
 * @param str 길이 정보를 가지는 문자열 관리 구조체(입력, 읽기 전용)
 * @return 성공 시 관리하는 문자열의 길이, 실패 시 0 반환
 */
int GetLength(const StringPtr str)
{
    if(str == NULL || str->data == NULL) return 0;
    return str->length;
}

/**
 * @fn char* GetPtr(StringPtr str)
 * @brief 구조체에서 관리하는 문자열의 주소를 반환하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(입력, 읽기 전용)
 * @return 성공 시 관리하는 문자열, 실패 시 NULL 반환
 */
char* GetPtr(const StringPtr str)
{
    if(str == NULL) return NULL;
    return str->data;
}

/**
 * @fn char* SetString(StringPtr str, const char *s)
 * @brief 구조체에서 관리하는 문자열을 지정한 문자열로 새로 설정하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @param s 새로 설정할 문자열(입력, 읽기 전용)
 * @return 성공 시 새로 설정된 문자열, 실패 시 NULL 반환
 */
char* SetString(StringPtr str, const char *s)
{
    if(str == NULL || s == NULL) return NULL;

	int sLength = (int)strlen(s);
	int length = (str->length == sLength) ? str->length : sLength;

	char *data = CloneCharArray(s, length);
    if(data == NULL) return NULL;

	if(str->data != NULL) free(str->data);
	str->data = data;
    str->length = length;

    return str->data;
}

/**
 * @fn char* ConvertToUpperString(StringPtr str)
 * @brief 구조체에서 관리하는 문자열을 모두 대문자로 변경하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @return 성공 시 대문자로 변경된 문자열, 실패 시 NULL 반환
 */
char* ConvertToUpperString(StringPtr str)
{
	if(str == NULL || str->data == NULL || str->length <= 0) return NULL;
	ChangeStringCase(str, toupper);
	return str->data;
}

/**
 * @fn char* ConvertToLowerString(StringPtr str)
 * @brief 구조체에서 관리하는 문자열을 모두 소문자로 변경하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @return 성공 시 소문자로 변경된 문자열, 실패 시 NULL 반환
 */
char* ConvertToLowerString(StringPtr str)
{
	if(str == NULL || str->data == NULL || str->length <= 0) return NULL;
	ChangeStringCase(str, tolower);
	return str->data;
}

/**
 * @fn char* TrimLeft(StringPtr str)
 * @brief 구조체에서 관리하는 문자열의 왼쪽 공백을 모두 제거하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @return 성공 시 왼쪽 공백이 제거된 문자열, 실패 시 NULL 반환
 */
char* TrimLeft(StringPtr str)
{
	if(str == NULL || str->data == NULL || str->length <= 0) return NULL;

	int strLength = str->length;
	int strIndex = 0;
	int leftSpaceCount = 0;

	for( ; strIndex < strLength; strIndex++)
	{
		if(IsSpace(str->data[strIndex]) == True) leftSpaceCount++;
		else break;
	}

	if(leftSpaceCount > 0)
	{
		str->length = strLength - leftSpaceCount;
		if(SetString(str, str->data + leftSpaceCount) == NULL)
		{
			return NULL;
		}
	}

	return str->data;
}

/**
 * @fn char* TrimRight(StringPtr str)
 * @brief 구조체에서 관리하는 문자열의 오른쪽 공백을 모두 제거하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @return 성공 시 오른쪽 공백이 제거된 문자열, 실패 시 NULL 반환
 */
char* TrimRight(StringPtr str)
{
	if(str == NULL || str->data == NULL || str->length <= 0) return NULL;

	int strLength = str->length;
	int strIndex = strLength - 1;
	int rightSpaceCount = 0;

	for( ; strIndex >= 0; strIndex--)
	{
		if(IsSpace(str->data[strIndex]) == True) rightSpaceCount++;
		else break;
	}

	if(rightSpaceCount > 0)
	{
		str->length = strLength - rightSpaceCount;
		if(SetString(str, str->data) == NULL)
		{
			return NULL;
		}
	}

	return str->data;
}

/**
 * @fn char* Trim(StringPtr str)
 * @brief 구조체에서 관리하는 문자열의 양쪽 공백을 모두 제거하는 함수
 * TrimRight 함수에서 전달받은 구조체 포인터와 관리하는 문자열의 NULL 체크를 수행하므로 별도로 체크하지 않는다.
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @return 성공 시 양쪽 공백이 제거된 문자열, 실패 시 NULL 반환
 */
char* Trim(StringPtr str)
{
	if(str == NULL || TrimRight(str) == NULL || TrimLeft(str) == NULL) return NULL;
	return str->data;
}

/**
 * @fn char* CopyString(StringPtr dstStr, const StringPtr srcStr)
 * @brief source 문자열 관리 구조체에서 destination 문자열 관리 구조체로 동일한 문자열을 복사하는 함수(길이도 포함)
 * @param dstStr 복사될 문자열 관리 구조체(출력)
 * @param srcStr 복사할 문자열 관리 구조체(입력, 읽기 전용)
 * @return 성공 시 복사된 문자열, 실패 시 NULL 반환
 */
char* CopyString(StringPtr dstStr, const StringPtr srcStr)
{
	if(dstStr == NULL || srcStr == NULL) return NULL;

	int dstStrLength = dstStr->length;
	int srcStrLength = srcStr->length;

	// srcStrLength 가 dstStrLength 와 같지 않으면 해당 길이만큼 새로운 문자열 생성
	if(dstStrLength != srcStrLength)
	{
		if(SetString(dstStr, srcStr->data) == NULL) return NULL;
	}
	// 그렇지 않다면, 생성할 필요 없이 srcStrLength 만큼 그대로 복사
	else
	{
		memcpy(dstStr->data, srcStr->data, (size_t)srcStrLength);
		dstStr->data[dstStr->length] = '\0';
	}

	return dstStr->data;
}

/**
 * @fn char* CopyNString(StringPtr dstStr, const StringPtr srcStr, int length)
 * @brief source 문자열 관리 구조체에서 destination 문자열 관리 구조체로 지정한 길이만큼 문자열을 복사하는 함수
 * @param dstStr 복사될 문자열 관리 구조체(출력)
 * @param srcStr 복사할 문자열 관리 구조체(입력, 읽기 전용)
 * @param length 복사할 길이(입력)
 * @return 성공 시 복사된 문자열, 실패 시 NULL 반환
 */
char* CopyNString(StringPtr dstStr, const StringPtr srcStr, int length)
{
	if(length <= 0 || dstStr == NULL || srcStr == NULL)
	{
		return NULL;
	}

	int dstStrLength = dstStr->length;
	int srcStrLength = srcStr->length;

	// srcStr 이 빈문자열이면 dstStr 을 빈문자열로 만들어서 반환
	if(srcStrLength == 0)
	{
		if(SetString(dstStr, "") == NULL) return NULL;
	}

	// 복사할 길이가 srcStrLength 보다 크면, srcStrLength 만큼 복사
	if(length > srcStrLength) length = srcStrLength;

	// 복사할 길이가 dstStrLength 와 같지 않으면 복사할 길이만큼 새로운 문자열 생성
	if(dstStrLength != length)
	{
		if(SetString(dstStr, srcStr->data) == NULL) return NULL;
	}
	// 그렇지 않다면, 생성할 필요 없이 복사할 길이만큼 그대로 복사
	else
	{
		memcpy(dstStr->data, srcStr->data, (size_t)length);
		dstStr->data[dstStr->length] = '\0';
	}

	return dstStr->data;
}

/**
 * @fn char* FormatString(StringPtr str, const char* format, ...)
 * @brief 지정한 출력 형식으로 문자열을 저장하는 함수
 * @param str 출력된 문자열을 저장할 문자열 관리 구조체(출력)
 * @param format 가변 문자열을 출력 형식을 저장한 문자열(입력, 읽기 전용)
 * @param ... 가변 문자열(입력)
 * @return 성공 시 출력된 문자열, 실패 시 NULL 반환
 */
char* FormatString(StringPtr str, const char* format, ...)
{
	if(str == NULL || format == NULL) return NULL;

	va_list ap;
	va_start(ap, format);

	int newLength = vsnprintf(NULL, 0, format, ap);
	if(newLength < 0) return NULL;
	newLength++;
	va_end(ap);

	char *newData = CloneCharArray("", newLength);
	if(newData == NULL) return NULL;

	va_start(ap, format);
	int copiedLength = vsnprintf(newData, (size_t)newLength, format, ap);
	if((strlen(newData) == 0)
		&& (copiedLength < 0 // glibc < 2.1
		 || (size_t)copiedLength >= sizeof(newData))) // glibc >= 2.1
	{
		free(newData);
		va_end(ap);
		return NULL;
	}
	va_end(ap);

	if(str->data != NULL) free(str->data);
	str->data = newData;
	str->length = (int)strlen(newData);

	return str->data;
}

/**
 * @fn char* ConcatString(StringPtr str, const char* s)
 * @brief 원본 문자열 뒤에 지정한 문자열을 붙이는 함수
 * @param str 덧붙여질 문자열을 저장할 문자열 관리 구조체(출력)
 * @param s 덧붙일 문자열(입력, 읽기 전용), NULL 이면 덧붙여질 문자열을 반환
 * @return 성공 시 덧붙여진 문자열, 실패 시 NULL 반환
 */
char* ConcatString(StringPtr str, const char* s)
{
	if(str == NULL || str->data == NULL || s == NULL) return NULL;

	int newLength = str->length + (int)strlen(s);
	char *newData = (char*)malloc((size_t)(newLength + 1));
	if(newData == NULL) return NULL;

	memcpy(newData, str->data, (size_t)(str->length));
	memcpy(newData + str->length, s, (size_t)strlen(s));
	newData[newLength] = '\0';

	free(str->data);
	str->data = newData;
	str->length = newLength;

	return str->data;
}

/**
 * @fn char* TruncateString(StringPtr str, int from)
 * @brief 문자열에서 지정한 위치부터 끝까지 잘라내는 함수
 * @param str 잘라낼 문자열을 저장할 문자열 관리 구조체(출력)
 * @param from 잘라낼 문자열의 시작 위치(입력)
 * @return 성공 시 잘라낸 문자열, 실패 시 NULL 반환
 */
char* TruncateString(StringPtr str, int from)
{
	if(str == NULL || str->data == NULL || str->length <= 0 || from < 0 || from >= str->length) return NULL;
	*(str->data + from) = '\0';
	SetString(str, str->data);
	return str->data;
}

/**
 * @fn StringPtr SubString(const StringPtr str, int from, int length)
 * @brief 문자열에서 지정한 위치부터 지정한 길이까지 추출해서 새로운 문자열을 생성하는 함수
 * @param str 추출할 문자열을 저장한 문자열 관리 구조체(입력, 읽기 전용)
 * @param from 추출할 문자열의 시작 위치(입력)
 * @param length 추출할 길이(입력)
 * @return 성공 시 추출한 문자열 구조체(cloned)의 주소, 실패 시 NULL 반환
 */
StringPtr SubString(const StringPtr str, int from, int length)
{
	if(str == NULL || str->data == NULL || str->length <= 0 || length < 0 || from < 0 || from >= str->length) return NULL;
	int to = from + length;
	if(to > str->length) to = str->length;

	StringPtr newStr = CloneString(str);
	*(newStr->data + to) = '\0';
	char *newData = newStr->data + from;
	SetString(newStr, newData);

	return newStr;
}

/**
 * @fn CompareResult CompareString(const StringPtr str1, const StringPtr str2)
 * @brief 두 문자열을 비교하는 함수
 * @param str1 비교될 문자열 관리 구조체(입력, 읽기 전용)
 * @param str2 비교할 문자열 관리 구조체(입력, 읽기 전용)
 * @return 두 문자열이 일치하면 0, 첫 번째 문자열이 더 크면 1, 작으면 -1, 실패 시 COMPARE_ERROR(-2) 반환
 */
int CompareString(const StringPtr str1, const StringPtr str2)
{
	int result = 0;
	if(str1 == NULL || str2 == NULL || str1->data == NULL || str2->data == NULL) return COMPARE_ERROR;

	// 빈문자열인 경우, 길이 비교
	if(str1->length <= 0 && str2->length > 0) return -1;
	else if(str1->length > 0 && str2->length <= 0) return 1;
	else if(str1->length == 0 && str2->length == 0) return 0;

	// 첫 번째 문자가 다를 경우, 해당 문자만 대소 비교
	// 같으면 아래 비교 로직 수행
	if(str1->data[0] < str2->data[0]) return -1;
	else if(str1->data[0] > str2->data[0]) return 1;

	// 첫 번째 문자가 같은 경우, 두 번째 문자부터 문자열 비교
	// 길이가 작은 문자열을 기준으로 비교
	int smallCount = 0;
	int bigCount = 0;
	int strIndex = 0;
	int compLength = (str1->length <= str2->length) ? str1->length : str2->length;
	char *str1Data = str1->data + 1;
	char *str2Data = str2->data + 1;

	for( ; strIndex < compLength; strIndex++)
	{
		if(*str1Data < *str2Data) smallCount++;
		else if(*str1Data > *str2Data) bigCount++;
		if(smallCount > 0 || bigCount > 0) break;

		str1Data++;
		str2Data++;
	}

	// 길이가 작은 문자열의 길이까지 비교했을 때 모두 일치하면 길이 비교
	if(smallCount == 0 && bigCount == 0)
	{
		if(str1->length < str2->length) result = -1;
		else if(str1->length > str2->length) result = 1;
	}
	// 일치하지 않으면 판단된 대소에 따라 결과값 반환
	else if(smallCount > 0) result = -1;
	else if(bigCount > 0) result = 1;

	return result;
}

/**
 * @fn Bool SearchString(const StringPtr str, const char *pattern)
 * @brief 지정한 문자열에서 특정 문자열을 검색하는 함수
 * @param str 검색될 문자열을 저장한 구조체(입력, 읽기 전용)
 * @param pattern 검색할 문자열(입력, 읽기 전용)
 * @return 문자열을 찾으면 True(1), 못찾으면 False(-1), 실패 시 SEARCH_ERROR(-2) 반환
 */
Bool SearchString(const StringPtr str, const char *pattern)
{
	Bool result = False;
	if(pattern == NULL || str == NULL || str->data == NULL || str->length <= 0) return SEARCH_ERROR;

	int patternLength = (int)strlen(pattern);
	if(patternLength == 0 || patternLength > str->length) return result;

	int strLength = str->length;
	int strIndex = 0;
	int patternIndex = 0;

	for( ; strIndex < strLength; strIndex++)
	{
		// 첫 문자를 먼저 검색
		if(str->data[strIndex] == pattern[patternIndex])
		{
			// 첫 문자가 일치하므로 1 부터 시작
			int equalCount = 1;
			// 검색할 문자열과 패턴 문자열이 일치하는 첫 문자의 위치부터 검색 시작
			int tempIndex = (++strIndex);
			patternIndex++;

			// 첫 문자가 일치하면 그 다음 문자들이 일치하는지 검사
			for( ; patternIndex < patternLength; patternIndex++)
			{
				if(str->data[tempIndex] != pattern[patternIndex]) break;
				else equalCount++;
				tempIndex++;
			}

			// 비교한 모든 문자들이 같으면 검색 성공
			if(equalCount == patternLength)
			{
				result = True;
				break;
			}
		}
		// 못찾았으면 다시 검색
		patternIndex = 0;
	}

	return result;
}

/**
 * @fn Bool IsDigit(char c)
 * @brief 지정한 문자가 숫자 문자인지 검사하는 함수
 * @param c 검사할 문자(입력)
 * @return 성공 시 True(1), 실패 시 False(-1) 반환
 */
Bool IsDigit(char c)
{
	return (c >= '0' && c <= '9') ? True : False;
}

/**
 * @fn Bool IsAlpha(char c)
 * @brief 지정한 문자가 알파벳 문자인지 검사하는 함수
 * @param c 검사할 문자(입력)
 * @return 성공 시 True(1), 실패 시 False(-1) 반환
 */
Bool IsAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ? True : False;
}

/**
 * @fn Bool IsLetter(char c)
 * @brief 지정한 문자가 알파벳 또는 '_'(underscore) 문자인지 검사하는 함수
 * @param c 검사할 문자(입력)
 * @return 성공 시 True(1), 실패 시 False(-1) 반환
 */
Bool IsLetter(char c)
{
	return (IsAlpha(c) == True || c == '_') ? True : False;
}

/**
 * @fn Bool IsSpace(char c)
 * @brief 지정한 문자가 공백 또는 탭문자인지 검사하는 함수
 * @param c 검사할 문자(입력)
 * @return 성공 시 True(1), 실패 시 False(-1) 반환
 */
Bool IsSpace(char c)
{
	return (c == ' ' || c == '\t') ? True : False;
}

/**
 * @fn Bool IsCRLF(char c)
 * @brief 지정한 문자가 CR('\r') 또는 LF('\n') 문자인지 검사하는 함수
 * @param c 검사할 문자(입력)
 * @return 성공 시 True(1), 실패 시 False(-1) 반환
 */
Bool IsCRLF(char c)
{
	return (c == '\r' || c == '\n') ? True : False;
}

/**
 * @fn char** SplitString(const char *s, char delimiter)
 * @brief 문자열을 지정한 구분 문자로 나누는 함수
 * @param s 나눌 문자열(입력, 읽기 전용)
 * @param delimiter 구분 문자(입력)
 * @param option 빈문자열이 포함된 배열을 포함할 것인지 선택 여부(입력, SplitOption 열거형 참고)
 * @return 성공 시 나눠진 문자열들의 주소를 저장한 새로운 문자열 배열, 실패 시 NULL 반환
 */
char** SplitString(const char *s, char delimiter, SplitOption option)
{
	if(s == NULL || strlen(s) == 0 || delimiter == '\0') return NULL;

	// 1) 문자열 내 delimiter 개수를 구하고 문자열 내 delimiter 위치를 저장한 배열을 생성한다.
	int delimiterCount = 0;
	int *delimiterPosArray = MakeDelimiterPosArray(s, delimiter, &delimiterCount);
	if(delimiterPosArray == NULL) return NULL;
	
	// 2) delimiter 로 분리될 문자열들을 저장하기 위한 배열을 생성한다.
	// (delimiter 개수 + 1) 만큼 포인터 배열의 크기를 생성 < +1 : (마지막 delimiter 뒤의 문자열)
	int strLength = (int)strlen(s);
	int strListIndex = 0;
	int strListLength = delimiterCount + 1;
	char **strList = NewCharPtrContainer(strListLength);
	if(strList == NULL) return NULL;

	// 3) delimiter 로 문자열을 분리해서 생성한 배열에 저장한다.
	for(strListIndex = 0; strListIndex < strListLength; strListIndex++)
	{
		int curLength = delimiterPosArray[strListIndex];
		if(strListIndex > 0){
			if(curLength != 0) curLength -= (delimiterPosArray[strListIndex - 1] + 1);
			// 마지막으로 분리될 문자열
			else curLength = strLength - (delimiterPosArray[strListIndex - 1] + 1);
			// delimiter 문자는 넘어감
			if(*s == delimiter) s++;
		}

		// 3-1-1) 빈문자열을 배열에 저장하지 않으면 해당 배열 위치에 NULL 을 저장한다.
		if(option == ExcludeEmptyString && curLength == 0) strList[strListIndex] = NULL;
		// 3-1-2) 모든 문자열을 저장한다.
		else
		{
			char *newStr = CloneCharArray(s, curLength);
			if(newStr == NULL){
				strList[strListIndex] = NULL;
				DeleteCharPtrContainer(strList);
				return NULL;
			}
			strList[strListIndex] = newStr;
		}
		s += curLength;
	}
	free(delimiterPosArray);

	// 3-2) 빈문자열을 배열에 저장하지 않으면 배열에서 NULL 이 아닌 포인터만 선택해서 다시 저장한다.
	// 빈문자열을 포함시키지 않는 경우
	if(option == ExcludeEmptyString)
	{
		return RemoveNullPointerInCharPtrContainer(strList, strListLength);
	}

	return strList;
}

/**
 * @fn char* MergeString(const char **sList, char delimiter)
 * @brief 문자열을 지정한 구분 문자로 연결하는 함수
 * 연결된 문자열은 기존 문자열가 아닌 새로 생성된 문자열
 * @param s 연결할 문자열들을 저장된 문자열 배열(입력)
 * @param delimiter 구분 문자(입력)
 * @return 성공 시 연결된 문자열, 실패 시 NULL 반환
 */
char* MergeString(char **sList, char delimiter)
{
	// 문자열이 NULL 또는 빈문자열인 경우, delimiter 가 널 문자인 경우, NULL 반환
	if(sList == NULL || delimiter == '\0') return NULL;

	int delimiterCount = 0;
	int strListLength = 0;
	int strLength = 0;
	char **tempList = sList;

	// 1) 추가해야할 delimiter 개수와 연결할 문자열의 총 길이를 구한다.
	// 연결할 문자열에는 (문자열 개수(>0) - 1)만큼 delimiter 가 포함되어야 한다.
	while(*tempList != NULL)
	{
		strLength += (int)strlen(*tempList);
		delimiterCount++;
		tempList++;
	}
	strListLength = delimiterCount;
	if(delimiterCount > 0) delimiterCount--;
	else return NULL;

	// 2) 연결할 문자열을 새로 생성한다.
	char *newStr = CloneCharArray("", strLength + delimiterCount);
	if(newStr == NULL) return NULL;

	// 3) 분리된 문자열을 delimiter 를 추가하면서 하나의 문자열로 연결한다.
	int strListIndex = 0;
	int totalStrLength = 0;
	for( ; strListIndex < strListLength; strListIndex++)
	{
		if(sList[strListIndex] != NULL){
			int curStrLength = (int)strlen(sList[strListIndex]);
			memcpy(newStr + totalStrLength, sList[strListIndex], (size_t)curStrLength);
			// 3-1) 마지막 문자열이면 뒤에 delimiter 를 포함하지 않는다.
			if(strListIndex < (strListLength - 1))
			{
				totalStrLength += curStrLength;
				newStr[totalStrLength++] = delimiter;
			}
		}
	}

	return newStr;
}

////////////////////////////////////////////////////////////////////////////////
/// Static Functions
////////////////////////////////////////////////////////////////////////////////

/**
 * @fn static StringPtr CreateString()
 * @brief 문자열 관리 구조체를 새로 생성하고 초기화하는 함수
 * @return 성공 시 문자열 관리 구조체의 주소, 실패 시 NULL 반환
 */
static StringPtr CreateString()
{
	StringPtr str = (StringPtr)malloc(sizeof(String));
	if(str == NULL) return NULL;
	str->data = NULL;
	str->length = 0;
	return str;
}

/**
 * @fn static char* CloneCharArray(const char *s, int length)
 * @brief 문자열을 복제하는 함수
 * 호출된 함수에서 전달받은 문자열과 문자열의 길이에 대한 예외 검사를 수행하였기 때문에 진행하지 않는다.
 * @param s 복제할 문자열(입력, 읽기 전용)
 * @param size 문자열의 길이(입력)
 * @return 성공 시 복제된 문자열, 실패 시 NULL 반환
 */
static char* CloneCharArray(const char *s, int length)
{
	char *newStr = (char*)malloc((size_t)(length + 1));
	if(newStr == NULL) return NULL;
	memcpy(newStr, s, (size_t)length);
	*(newStr + length) = '\0';
	return newStr;
}

/**
 * @fn static char** RemoveNullPointerInCharPtrContainer(char **container, int size)
 * @brief 문자열 배열에서 널 포인터가 아닌 문자열 주소만 새로 저장하도록 하는 함수
 * SplitString 함수에서 호출되기 때문에 전달받은 문자열 배열과 크기에 대한 예외 검사를 수행하지 않는다.
 * @param container 널 포인터를 삭제할 문자열 배열(출력)
 * @param size 문자열 배열의 크기(입력)
 * @return 성공 시 문자열 배열, 실패 시 NULL 반환
 */
static char** RemoveNullPointerInCharPtrContainer(char **container, int size)
{
	int pointerCount = 0;
	int containerIndex = 0;

	for( ; containerIndex < size; containerIndex++)
	{
		if(container[containerIndex] != NULL) pointerCount++;
	}

	int newContainerSize = pointerCount;
	char **newContainer = NewCharPtrContainer(newContainerSize);
	if(newContainer == NULL) return container;

	int newContainerIndex = 0;
	for(containerIndex = 0; containerIndex < size; containerIndex++)
	{
		if(container[containerIndex] != NULL)
		{
			newContainer[newContainerIndex] = container[containerIndex];
			newContainerIndex++;
		}
	}
	newContainer[newContainerSize] = NULL;

	free(container);
	return newContainer;
}

/**
 * @fn static int* MakeDelimiterPosArray(const char *s, char delimiter, int *delimiterCount)
 * @brief 문자열에서 지정한 delimiter 의 위치를 저장한 배열을 반환하는 함수
 * SplitString 함수에서 호출되기 때문에 전달받은 문자열과 delimiter, delimiterCount 에 대한 예외 검사를 수행하지 않는다.
 * @param s delimiter 를 검사할 문자열(입력, 읽기 전용)
 * @param delimiter 구분 문자(입력)
 * @param delimiterCount 구분 문자 개수(출력)
 * @return 성공 시 delimiter 위치를 저장한 배열, 실패 시 NULL 반환
 */
static int* MakeDelimiterPosArray(const char *s, char delimiter, int *delimiterCount)
{
	int strLength = (int)strlen(s);
	int strIndex = 0;
	int delimiterIndex = 0;

	for( ; strIndex < strLength; strIndex++)
	{
		if(s[strIndex] == delimiter) (*delimiterCount)++;
	}

	// delimiter 가 문자열에 없으면 실패, NULL 반환
	if(*delimiterCount == 0) return NULL;

	int *delimiterPosArray = (int*)calloc(sizeof(int) * (size_t)(*delimiterCount), sizeof(int));
	if(delimiterPosArray == NULL) return NULL;

	for(strIndex = 0; strIndex < strLength; strIndex++)
	{
		if(s[strIndex] == delimiter) delimiterPosArray[delimiterIndex++] = strIndex;
	}

	return delimiterPosArray;
}

/**
 * @fn static char* ChangeStringCase(StringPtr str, CharFunction_f func)
 * @brief 구조체에서 관리하는 문자열을 특정 문자 Case 로 변경하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @param func 문자의 case 를 변경하기 위한 함수(입력)
 * @return 성공 시 변경된 문자열, 실패 시 NULL 반환
 */
static char* ChangeStringCase(StringPtr str, CharFunction_f func)
{
	int strLength = str->length;
	int strIndex = 0;

	for( ; strIndex < strLength; strIndex++)
	{
		str->data[strIndex] = (char)func(str->data[strIndex]);
	}

	return str->data;
}

////////////////////////////////////////////////////////////////////////////////
/// Util Functions
////////////////////////////////////////////////////////////////////////////////

/**
 * @fn char** NewCharPtrContainer(int size)
 * @brief 문자열 배열을 새로 생성하고 초기화하는 함수
 * @param size 새로 생성할 문자열 배열의 크기(입력)
 * @return 성공 시 생성된 문자열 배열, 실패 시 NULL 반환
 */
char** NewCharPtrContainer(int size)
{
	char **newContainer = (char**)malloc(sizeof(char*) * (size_t)(size + 1));
	if(newContainer == NULL) return NULL;
	newContainer[size] = NULL;
	return newContainer;
}

/**
 * @fn void DeleteCharPtrContainer(char **container)
 * @brief 문자열 배열을 삭제하는 함수
 * @param container 삭제할 문자열 배열(입력)
 * @return 반환값 없음
 */
void DeleteCharPtrContainer(char **container)
{
    char **tempContainer = container;
    while(*tempContainer != NULL)
    {
        free(*tempContainer);
        tempContainer++;
    }
    free(container);
}


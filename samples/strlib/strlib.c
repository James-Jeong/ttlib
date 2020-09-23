#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "strlib.h"

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
    if(s == NULL)
    {
        return NULL;
    }

    StringPtr newString = (String*)malloc(sizeof(String));
    if(newString == NULL)
    {
        return NULL;
    }
    
    int length = (int)strlen(s);
    newString->data = (char*)malloc((size_t)length + 1);
    if(newString->data == NULL)
    {
        free(newString);
        return NULL;
    }

    memcpy(newString->data, s, (size_t)length);
    *(newString->data + length) = '\0';

    newString->length = length;

    return newString;
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
 * @return 성공 시 새로 복제된 String 객체 주소, 실패 시 NULL 반환
 */
StringPtr CloneString(const StringPtr str)
{
    if(str == NULL)
    {
        return NULL;
    }

    int newLength = str->length;
    char *newData = (char*)malloc((size_t)newLength + 1);
    if(newData == NULL)
    {
        return NULL;
    }

    memcpy(newData, str->data, (size_t)newLength);
    *(newData + newLength) = '\0';

    StringPtr newString = (String*)malloc(sizeof(String));
    if(newString == NULL)
    {
        free(newData);
        return NULL;
    }

    newString->data = newData;
    newString->length = newLength;

    return newString;
}

/**
 * @fn int GetLength(const StringPtr str)
 * @brief 구조체에서 관리하는 문자열의 길이를 반환하는 함수
 * @param str 길이 정보를 가지는 문자열 관리 구조체(입력, 읽기 전용)
 * @return 성공 시 관리하는 문자열의 길이, 실패 시 0 반환
 */
int GetLength(const StringPtr str)
{
    if(str == NULL)
    {
        return 0;
    }

    return str->length;
}

/**
 * @fn char* GetPtr(StringPtr str)
 * @brief 구조체에서 관리하는 문자열의 주소를 반환하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(입력, 읽기 전용)
 * @return 성공 시 관리하는 문자열의 주소, 실패 시 NULL 반환
 */
char* GetPtr(const StringPtr str)
{
    if(str == NULL)
    {
        return NULL;
    }

    return str->data;
}

/**
 * @fn char* SetString(StringPtr str, char *s)
 * @brief 구조체에서 관리하는 문자열을 지정한 문자열로 새로 설정하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @param s 새로 설정할 문자열(입력, 읽기 전용)
 * @return 성공 시 새로 설정된 문자열의 주소, 실패 시 NULL 반환
 */
char* SetString(StringPtr str, const char *s)
{
    if(str == NULL || s == NULL)
    {
        return NULL;
    }

    int length = (int)strlen(s);
    char *newData = (char*)malloc((size_t)length + 1);
    if(newData == NULL)
    {
        return NULL;
    }

    memcpy(newData, s, (size_t)length);
    *(newData + length) = '\0';

    free(str->data);
    str->data = newData;
    str->length = length;

    return str->data;
}

/**
 * @fn char* ConvertToUpperCase(StringPtr str)
 * @brief 구조체에서 관리하는 문자열을 모두 대문자로 변경하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @return 성공 시 대문자로 변경된 문자열의 주소, 실패 시 NULL 반환
 */
char* ConvertToUpperCase(StringPtr str)
{
	if(str == NULL)
	{
		return NULL;
	}

	if(str->data == NULL)
	{
		return NULL;
	}

	int strLength = str->length;
	int strIndex = 0;

	for( ; strIndex < strLength; strIndex++)
	{
		str->data[strIndex] = (char)toupper(str->data[strIndex]);
	}

	return str->data;
}

/**
 * @fn char* ConvertToLowerCase(StringPtr str)
 * @brief 구조체에서 관리하는 문자열을 모두 소문자로 변경하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @return 성공 시 소문자로 변경된 문자열의 주소, 실패 시 NULL 반환
 */
char* ConvertToLowerCase(StringPtr str)
{
	if(str == NULL)
	{
		return NULL;
	}

	if(str->data == NULL)
	{
		return NULL;
	}

	int strLength = str->length;
	int strIndex = 0;

	for( ; strIndex < strLength; strIndex++)
	{
		str->data[strIndex] = (char)tolower(str->data[strIndex]);
	}

	return str->data;
}

/**
 * @fn char* RemoveLeftSpace(StringPtr str)
 * @brief 구조체에서 관리하는 문자열의 왼쪽 공백을 모두 제거하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @return 성공 시 왼쪽 공백이 제거된 문자열의 주소, 실패 시 NULL 반환
 */
char* RemoveLeftSpace(StringPtr str)
{
	if(str == NULL)
	{
		return NULL;
	}

	if(str->data == NULL)
	{
		return NULL;
	}

	int strLength = str->length;
	int strIndex = 0;
	int leftSpaceCount = 0;

	for( ; strIndex < strLength; strIndex++)
	{
		if(isspace(str->data[strIndex]) != 0)
		{
			leftSpaceCount++;
		}
		else break;
	}

	if(leftSpaceCount > 0)
	{
		int newDataLength = strLength - leftSpaceCount + 1;
		char *newData = (char*)malloc((size_t)newDataLength);
		if(newData == NULL)
		{
			return NULL;
		}

		memcpy(newData, str->data + leftSpaceCount, (size_t)newDataLength);
		*(newData + newDataLength) = '\0';

		free(str->data);
		str->data = newData;
	}

	return str->data;
}

/**
 * @fn char* RemoveRightSpace(StringPtr str)
 * @brief 구조체에서 관리하는 문자열의 오른쪽 공백을 모두 제거하는 함수
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @return 성공 시 오른쪽 공백이 제거된 문자열의 주소, 실패 시 NULL 반환
 */
char* RemoveRightSpace(StringPtr str)
{
	if(str == NULL)
	{
		return NULL;
	}

	if(str->data == NULL)
	{
		return NULL;
	}

	int strLength = str->length;
	int strIndex = strLength - 1;
	int rightSpaceCount = 0;

	for( ; strIndex >= 0; strIndex--)
	{
		if(isspace(str->data[strIndex]) != 0)
		{
			rightSpaceCount++;
		}
		else break;
	}

	if(rightSpaceCount > 0)
	{
		int newDataLength = strLength - rightSpaceCount + 1;
		char *newData = (char*)malloc((size_t)newDataLength);
		if(newData == NULL)
		{
			return NULL;
		}

		str->data[strLength - rightSpaceCount] = '\0';
		memcpy(newData, str->data, (size_t)newDataLength);

		free(str->data);
		str->data = newData;
	}

	return str->data;
}

/**
 * @fn char* RemoveBothSpace(StringPtr str)
 * @brief 구조체에서 관리하는 문자열의 양쪽 공백을 모두 제거하는 함수
 * RemoveRightSpace 함수에서 전달받은 구조체 포인터와 관리하는 문자열의 NULL 체크를 수행하므로 별도로 체크하지 않는다.
 * @param str 문자열의 정보를 가지는 문자열 관리 구조체(출력)
 * @return 성공 시 양쪽 공백이 제거된 문자열의 주소, 실패 시 NULL 반환
 */
char* RemoveBothSpace(StringPtr str)
{
	//TODO Clone -> set -> return str->data
	char *trimmedString = RemoveRightSpace(str);
	if(trimmedString == NULL)
	{
		return NULL;
	}

	if(SetString(str, trimmedString) == NULL)
	{
		return NULL;
	}
	
	return RemoveLeftSpace(str);
}

/**
 * @fn char* CopyString(StringPtr dstStr, const StringPtr srcStr)
 * @brief source 문자열 관리 구조체에서 destination 문자열 관리 구조체로 동일한 문자열을 복사하는 함수(길이도 포함)
 * @param dstStr 복사될 문자열 관리 구조체(출력)
 * @param srcStr 복사할 문자열 관리 구조체(입력, 읽기 전용)
 * @return 성공 시 복사된 문자열의 주소, 실패 시 NULL 반환
 */
char* CopyString(StringPtr dstStr, const StringPtr srcStr)
{
	if(dstStr == NULL || srcStr == NULL)
	{
		return NULL;
	}

	if(dstStr->data == NULL || srcStr->data == NULL)
	{
		return NULL;
	}

	int dstStrLength = dstStr->length;
	int srcStrLength = srcStr->length;

	// srcStrLength 가 dstStrLength 와 같지 않으면 해당 길이만큼 새로운 문자열 생성
	if(dstStrLength != srcStrLength)
	{
		char *newData = (char*)malloc((size_t)srcStrLength + 1);
		if(newData == NULL)
		{
			return NULL;
		}
		memcpy(newData, srcStr->data, (size_t)srcStrLength);

		free(dstStr->data);
		dstStr->data = newData;
	}
	// 그렇지 않다면, 생성할 필요 없이 srcStrLength 만큼 그대로 복사
	else
	{
		memcpy(dstStr->data, srcStr->data, (size_t)srcStrLength);
	}

	dstStr->length = srcStrLength;
	dstStr->data[dstStr->length] = '\0';

	return dstStr->data;
}

/**
 * @fn char* CopyNString(StringPtr dstStr, const StringPtr srcStr, int length)
 * @brief source 문자열 관리 구조체에서 destination 문자열 관리 구조체로 지정한 길이만큼 문자열을 복사하는 함수
 * @param dstStr 복사될 문자열 관리 구조체(출력)
 * @param srcStr 복사할 문자열 관리 구조체(입력, 읽기 전용)
 * @param length 복사할 길이(입력)
 * @return 성공 시 복사된 문자열의 주소, 실패 시 NULL 반환
 */
char* CopyNString(StringPtr dstStr, const StringPtr srcStr, int length)
{
	if(length <= 0)
	{
		return NULL;
	}

	if(dstStr == NULL || srcStr == NULL)
	{
		return NULL;
	}

	if(dstStr->data == NULL || srcStr->data == NULL)
	{
		return NULL;
	}

	int dstStrLength = dstStr->length;
	int srcStrLength = srcStr->length;

	if(srcStrLength == 0)
	{
		char *newData = realloc(dstStr->data, 1);
		if(newData == NULL)
		{
			return NULL;
		}
		dstStr->data = newData;
		dstStr->data[0] = '\0';
		dstStr->length = 0;
		return dstStr->data;
	}

	// 복사할 길이가 srcStrLength 보다 크면, srcStrLength 만큼 복사
	if(srcStrLength < length)
	{
		length = srcStrLength;
	}

	// 복사할 길이가 dstStrLength 와 같지 않으면 복사할 길이만큼 새로운 문자열 생성
	if(dstStrLength != length)
	{
		char *newData = (char*)malloc((size_t)length + 1);
		if(newData == NULL)
		{
			return NULL;
		}
		memcpy(newData, srcStr->data, (size_t)length);

		free(dstStr->data);
		dstStr->data = newData;
	}
	// 그렇지 않다면, 생성할 필요 없이 복사할 길이만큼 그대로 복사
	else
	{
		memcpy(dstStr->data, srcStr->data, (size_t)length);
	}

	dstStr->length = length;
	dstStr->data[dstStr->length] = '\0';

	return dstStr->data;
}

/**
 * @fn char* FormatString(StringPtr str, const char* format, ...)
 * @brief 지정한 출력 형식으로 문자열을 저장하는 함수
 * @param str 출력된 문자열을 저장할 문자열 관리 구조체(출력)
 * @param format 가변 문자열을 출력 형식을 저장한 문자열(입력, 읽기 전용)
 * @param ... 가변 문자열(입력)
 * @return 성공 시 출력된 문자열의 주소, 실패 시 NULL 반환
 */
char* FormatString(StringPtr str, const char* format, ...)
{
	if(str == NULL || format == NULL)
	{
		return NULL;
	}

	va_list ap;

	va_start(ap, format);

	int newLength = vsnprintf(NULL, 0, format, ap);
	if(newLength < 0)
	{
		return NULL;
	}
	newLength++;
	va_end(ap);

	char *newData = (char*)calloc((size_t)newLength, sizeof(char));
	if(newData == NULL)
	{
		return NULL;
	}

	va_start(ap, format);
	int copiedLength = vsnprintf(newData, (size_t)newLength, format, ap);
	if(copiedLength < 0 // glibc < 2.1
		 || (size_t)copiedLength >= sizeof(newData)) // glibc >= 2.1
	{
		free(newData);
		return NULL;
	}
	newData[newLength] = '\0';
	va_end(ap);

	free(str->data);
	str->data = newData;
	str->length = newLength;

	return str->data;
}

/**
 * @fn char* ConcatString(StringPtr str, const char* s)
 * @brief 원본 문자열 뒤에 지정한 문자열을 붙이는 함수
 * @param str 덧붙여질 문자열을 저장할 문자열 관리 구조체(출력)
 * @param s 덧붙일 문자열(입력, 읽기 전용)
 * @return 성공 시 덧붙여진 문자열의 주소, 실패 시 NULL 반환
 */
char* ConcatString(StringPtr str, const char* s)
{
	if(str == NULL || str->data == NULL)
	{
		return NULL;
	}

	if(s == NULL)
	{
		return str->data;
	}

	int newLength = str->length + (int)strlen(s);
	char *newData = (char*)malloc((size_t)newLength + 1);
	if(newData == NULL)
	{
		return NULL;
	}

	memcpy(newData, str->data, (size_t)str->length);
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
 * @return 성공 시 잘라낸 문자열의 주소, 실패 시 NULL 반환
 */
char* TruncateString(StringPtr str, int from)
{
	if(str == NULL || str->data == NULL || str->length <= 0 || from < 0 || from >= str->length)
	{
		return NULL;
	}

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
	if(str == NULL || str->data == NULL || str->length <= 0 || length < 0 || from < 0 || from >= str->length)
	{
		return NULL;
	}

	int to = from + length;
	if(to > str->length)
	{
		to = str->length;
	}

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

	if(str1 == NULL || str2 == NULL || str1->data == NULL || str2->data == NULL)
	{
		return COMPARE_ERROR;
	}

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
 * @fn SearchResult SearchString(const StringPtr str, const char *pattern)
 * @brief 지정한 문자열에서 특정 문자열을 검색하는 함수
 * @param str 검색될 문자열을 저장한 구조체(입력, 읽기 전용)
 * @param pattern 검색할 문자열(입력, 읽기 전용)
 * @return 성공 시 SearchTrue(1), 실패 시 SearchFalse(-1) 반환
 */
SearchResult SearchString(const StringPtr str, const char *pattern)
{
	SearchResult result = SearchFalse;

	if(pattern == NULL || str == NULL || str->data == NULL || str->length <= 0)
	{
		return result;
	}

	int patternLength = (int)strlen(pattern);
	if(patternLength == 0 || patternLength > str->length)
	{
		return result;
	}

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
				result = SearchTrue;
				break;
			}
		}
		// 못찾았으면 다시 검색
		patternIndex = 0;
	}

	return result;
}

/**
 * @fn CBool IsDigit(char c)
 * @brief 지정한 문자가 숫자 문자인지 검사하는 함수
 * @param c 검사할 문자(입력)
 * @return 성공 시 CTrue(1), 실패 시 CFalse(-1) 반환
 */
CBool IsDigit(char c)
{
	return (c >= '0' && c <= '9') ? CTrue : CFalse;
}

/**
 * @fn CBool IsAlpha(char c)
 * @brief 지정한 문자가 알파벳 문자인지 검사하는 함수
 * @param c 검사할 문자(입력)
 * @return 성공 시 CTrue(1), 실패 시 CFalse(-1) 반환
 */
CBool IsAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ? CTrue : CFalse;
}

CBool IsLetter(char c)
{
	return (IsAlpha(c) == CTrue || c == '_') ? CTrue : CFalse;
}

CBool IsSpace(char c)
{
	return (c == ' ' || c == '\t') ? CTrue : CFalse;
}

CBool IsCRLF(char c)
{
	return (c == '\r' || c == '\n') ? CTrue : CFalse;
}

char** SplitString(const char *s, char delimit)
{
	char **strList;



	return strList;
}

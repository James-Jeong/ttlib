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
    
    size_t length = strlen(s);
    newString->data = (char*)malloc(length + 1);
    if(newString->data == NULL)
    {
        free(newString);
        return NULL;
    }

    memcpy(newString->data, s, length);
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

    size_t newLength = str->length;
    char *newData = (char*)malloc(newLength + 1);
    if(newData == NULL)
    {
        return NULL;
    }

    memcpy(newData, str->data, newLength);
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
 * @fn size_t GetLength(const StringPtr str)
 * @brief 구조체에서 관리하는 문자열의 길이를 반환하는 함수
 * @param str 길이 정보를 가지는 문자열 관리 구조체(입력, 읽기 전용)
 * @return 성공 시 관리하는 문자열의 길이, 실패 시 0 반환
 */
size_t GetLength(const StringPtr str)
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

    size_t length = strlen(s);
    char *newData = (char*)malloc(length + 1);
    if(newData == NULL)
    {
        return NULL;
    }

    memcpy(newData, s, length);
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

	size_t strLength = str->length;
	size_t strIndex = 0;

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

	size_t strLength = str->length;
	size_t strIndex = 0;

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

	size_t strLength = str->length;
	size_t strIndex = 0;
	size_t leftSpaceCount = 0;

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
		size_t newDataLength = strLength - leftSpaceCount + 1;
		char *newData = (char*)malloc(newDataLength);
		if(newData == NULL)
		{
			return NULL;
		}

		memcpy(newData, str->data + leftSpaceCount, newDataLength);
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

	size_t strLength = str->length;
	int strIndex = (int)strLength - 1;
	size_t rightSpaceCount = 0;

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
		size_t newDataLength = strLength - rightSpaceCount + 1;
		char *newData = (char*)malloc(newDataLength);
		if(newData == NULL)
		{
			return NULL;
		}

		str->data[strLength - rightSpaceCount] = '\0';
		memcpy(newData, str->data, newDataLength);

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

	size_t dstStrLength = dstStr->length;
	size_t srcStrLength = srcStr->length;

	if(dstStrLength < srcStrLength)// dstStr->data 를 srcStr->data 의 길이와 같은 문자열로 새로 생성한다.
	{
		char *newData = (char*)malloc(srcStrLength);
		if(newData == NULL)
		{
			return NULL;
		}

		free(dstStr->data);
		dstStr->data = newData;
	}

	if(dstStrLength > srcStrLength) memset(dstStr->data, 0, dstStrLength);
	else memset(dstStr->data, 0, srcStrLength);
	memcpy(dstStr->data, srcStr->data, srcStrLength);
	dstStr->length = srcStrLength;

	return dstStr->data;
}

char* CopyNString(StringPtr dstStr, const StringPtr srcStr, size_t length)
{
	if(length == 0)
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

	size_t dstStrLength = dstStr->length;
	size_t srcStrLength = srcStr->length;

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

	if(srcStrLength < length) // 복사할 길이가 srcStrLength 보다 크면, srcStrLength 만큼 복사
	{
		length = srcStrLength;
	}

	if(dstStrLength < length) // dstStrLength 가 복사할 길이보다 작으면, dstStr->data 를 복사할 길이와 같은 문자열로 새로 생성한다.
	{
		char *newData = (char*)malloc(length);
		if(newData == NULL)
		{
			return NULL;
		}

		free(dstStr->data);
		dstStr->data = newData;
	}

	if(dstStrLength > length) memset(dstStr->data, 0, dstStrLength);
	else memset(dstStr->data, 0, length);
	memcpy(dstStr->data, srcStr->data, length);
	dstStr->length = length;

	return dstStr->data;

}

char* FormatString(StringPtr str, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	int newLength = vsnprintf(NULL, 0, format, ap) + 1;
	if(newLength < 1)
	{
		return NULL;
	}

	char *newData = (char*)calloc((size_t)newLength, sizeof(char));
	if(newData == NULL)
	{
		return NULL;
	}

	int result = vsnprintf(newData, sizeof(newData), format, ap);
	if(result < 0 // glibc < 2.1
		 || (size_t)result >= sizeof(newData)) // glibc >= 2.1
	{
		free(newData);
		return NULL;
	}

	va_end(ap);

	free(str->data);
	str->data = newData;
	str->length = (size_t)newLength;

	return str->data;
}

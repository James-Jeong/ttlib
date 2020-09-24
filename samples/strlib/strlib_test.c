#include "ttlib.h"
#include "strlib.h"

DECLARE_TEST();

TEST(NewString, InstanceCreation, {
    char *s = "my string";
    char *expected = s;
    StringPtr str = NewString(s);

	// NULL 이 아님을 확인
    EXPECT_NOT_NULL(str);

	// 기본적인 함수 동작 테스트
    EXPECT_STR_EQUAL(str->data, expected);    
    DeleteString(&str);

    // NULL 값이 들어온 경우, 0을 반환
    str = NewString(NULL);
    EXPECT_NULL(str);
})

TEST(DeleteString, InstanceDeletion, {
    StringPtr str = NewString("abcd");

	// 기본적인 함수 동작 테스트
    DeleteString(&str);

	// NULL 설정 확인
    EXPECT_NULL(str);
})

TEST(GetLength, LengthOfString, {
    char *s = "abcd";
    StringPtr str = NewString(s);
    int expected = (int)strlen(s);

	// 반환값 확인 및 기본적인 함수 동작 테스트
    EXPECT_NUM_EQUAL(GetLength(str), expected);
    DeleteString(&str);

    // 빈 문자열에 대해서, 0을 반환
    s = "";
    str = NewString(s);
    expected = 0;
    EXPECT_NUM_EQUAL(GetLength(str), expected);
    DeleteString(&str);

    // NULL 값이 들어온 경우, 0을 반환
    expected = 0;
    EXPECT_NUM_EQUAL(GetLength(NULL), expected);
})

TEST(GetPtr, PointerToStringData, {
    char *s = "abcd";
    StringPtr str = NewString(s);

    EXPECT_PTR_EQUAL(GetPtr(str), str->data);
    DeleteString(&str);

    // NULL 값이 들어온 경우, NULL 반환
    EXPECT_NULL(GetPtr(NULL));

})

TEST(CloneString, InstanceClone, {
    char *s = "abcd";
    StringPtr str1 = NewString(s);
    StringPtr str2 = CloneString(str1);

	// 반환값 확인
    EXPECT_NOT_NULL(str2);

	// 기본적인 복제 테스트
    EXPECT_STR_EQUAL(str1->data, str2->data);
    EXPECT_PTR_NOT_EQUAL(str1->data, str2->data);
    EXPECT_NUM_EQUAL(GetLength(str1), GetLength(str2));
    DeleteString(&str1);
    DeleteString(&str2);

	// NULL 값이 들어온 경우, NULL 반환
    str2 = CloneString(NULL);
    EXPECT_NULL(str2);
})

TEST(SetString, SetNewValue, {
    char *s1 = "abcd";
    char *s2 = "xyz";
    char *expected = s2;
    StringPtr str = NewString(s1);

	// 기본적인 문자열 설정 테스트
    EXPECT_STR_EQUAL(SetString(str, expected), expected);

	// NULL 값이 들어온 경우, NULL 반환
    EXPECT_NULL(SetString(str, NULL));
    EXPECT_NULL(SetString(NULL, expected));

    // 빈 문자열을 정상처리해야 한다.
    expected = "";
    EXPECT_STR_EQUAL(SetString(str, expected), expected);
    EXPECT_NUM_EQUAL(GetLength(str), (int)strlen(expected));

    DeleteString(&str);
})

TEST(ConvertToUpperCase, UpperString, {
	char *s = "abcd";
	char *expected = "ABCD"; 
    StringPtr str = NewString(s);

	// 반환값 확인
	EXPECT_NOT_NULL(ConvertToUpperCase(str));

	// 기본적인 변환 테스트
	s = "abcd";
    SetString(str, s);
	EXPECT_STR_EQUAL(ConvertToUpperCase(str), expected);

    // 빈 문자열을 정상처리해야 한다.
	s = "";
	expected = "";
    SetString(str, s);
	EXPECT_STR_EQUAL(ConvertToUpperCase(str), expected);

	s = "123-*ab-c+d(ef)";
	expected = "123-*AB-C+D(EF)";
	SetString(str, s);
	EXPECT_STR_EQUAL(ConvertToUpperCase(str), expected);

	// NULL 값이 들어온 경우, NULL 반환
	EXPECT_NULL(ConvertToUpperCase(NULL));

    DeleteString(&str);
})

TEST(ConvertToLowerCase, LowerString, {
	char *s = "ABCD";
	char *expected = "abcd"; 
    StringPtr str = NewString(s);

	// 반환값 확인
	EXPECT_NOT_NULL(ConvertToLowerCase(str));

	// 기본적인 변환 테스트
	s = "ABCD";
	SetString(str, s);
	EXPECT_STR_EQUAL(ConvertToLowerCase(str), expected);

    // 빈 문자열을 정상처리해야 한다.
	s = "";
	expected = "";
    SetString(str, s);
	EXPECT_STR_EQUAL(ConvertToLowerCase(str), expected);

	s = "123-*AB-C+D(EF)";
	expected = "123-*ab-c+d(ef)";
	SetString(str, s);
	EXPECT_STR_EQUAL(ConvertToLowerCase(str), expected);

	// NULL 값이 들어온 경우, NULL 반환
	EXPECT_NULL(ConvertToLowerCase(NULL));

    DeleteString(&str);
})

TEST(TrimString, RemoveLeftSpace, {
	char *s = " \tabcde";
	char *expected = "abcde"; 
    StringPtr str = NewString(s);

	// 반환값 확인
	EXPECT_NOT_NULL(RemoveLeftSpace(str));

	// 왼쪽 공백만 있는 경우
	s = " \t\t \tabcde";
    SetString(str, s);
	EXPECT_STR_EQUAL(RemoveLeftSpace(str), expected);

	// 왼쪽, 오른쪽 공백과 문자열 중간에 공백이 있는 경우
	s = " \ta b c d e   ";
	expected = "a b c d e   ";
    SetString(str, s);
	EXPECT_STR_EQUAL(RemoveLeftSpace(str), expected);

	// 빈 문자열을 정상처리해야 한다.
	s = "";
	expected = "";
    SetString(str, s);
	EXPECT_STR_EQUAL(RemoveLeftSpace(str), expected);

	// NULL 값이 들어온 경우, NULL 반환
	EXPECT_NULL(RemoveLeftSpace(NULL));

    DeleteString(&str);
})

TEST(TrimString, RemoveRightSpace, {
	char *s = "abcde \t";
	char *expected = "abcde"; 
    StringPtr str = NewString(s);

	// 반환값 확인
	EXPECT_NOT_NULL(RemoveRightSpace(str));

	// 오른쪽 공백만 있는 경우
	s = "abcde \t\t \t";
    SetString(str, s);
	EXPECT_STR_EQUAL(RemoveRightSpace(str), expected);

	s = " a b c d e \t\t \t";
	expected = " a b c d e";
    SetString(str, s);
	EXPECT_STR_EQUAL(RemoveRightSpace(str), expected);

	// 빈 문자열을 정상처리해야 한다.
	s = "";
	expected = "";
    SetString(str, s);
	EXPECT_STR_EQUAL(RemoveRightSpace(str), expected);

	// NULL 값이 들어온 경우, NULL 반환
	EXPECT_NULL(RemoveRightSpace(NULL));

    DeleteString(&str);
})

TEST(TrimString, RemoveBothSpace, {
	char *s = "\t abcde \t";
	char *expected = "abcde"; 
    StringPtr str = NewString(s);

	// 반환값 확인
	EXPECT_NOT_NULL(RemoveBothSpace(str));

	// 양쪽에 공백이 있는 경우
	s = "\t \t\t abcde \t\t \t";
     SetString(str, s);
	EXPECT_STR_EQUAL(RemoveBothSpace(str), expected);

	// 양쪽에 공백이 있고 문자열 중간에 공백이 있는 경우
	s = "\t \t\t a b c d e \t\t \t";
	expected = "a b c d e";
	SetString(str, s);
	EXPECT_STR_EQUAL(RemoveBothSpace(str), expected);

	// 빈 문자열을 정상처리해야 한다.
	s = "";
	expected = "";
    SetString(str, s);
	EXPECT_STR_EQUAL(RemoveBothSpace(str), expected);

	// NULL 값이 들어온 경우, NULL 반환
	EXPECT_NULL(RemoveBothSpace(NULL));

    DeleteString(&str);
})

TEST(CopyString, CopyString, {
	char *srcData = "abc";
	char *dstData = "def";
	char *expected = "abc"; 
    StringPtr srcStr = NewString(srcData);
	StringPtr dstStr = NewString(dstData);

	// 반환값 확인
	EXPECT_NOT_NULL(CopyString(dstStr, srcStr));

	// destination 문자열의 길이와 source 문자열의 길이가 같은 경우
	dstData = "def";
	SetString(dstStr, dstData);
	EXPECT_STR_EQUAL(CopyString(dstStr, srcStr), expected);

	// destination 문자열의 길이가 source 문자열의 길이보다 큰 경우 정상 동작해야 한다.
	dstData = "defghi";
	SetString(dstStr, dstData);
	EXPECT_STR_EQUAL(CopyString(dstStr, srcStr), expected);

	// destination 문자열의 길이가 source 문자열의 길이보다 작은 경우에도 정상 동작해야 한다.
	dstData = "d";
	SetString(dstStr, dstData);
	EXPECT_STR_EQUAL(CopyString(dstStr, srcStr), expected);

	// 빈 문자열을 정상적으로 복사해야 한다.
	srcData = "";
	expected = "";
    SetString(srcStr, srcData);
	EXPECT_STR_EQUAL(CopyString(dstStr, srcStr), expected);

	// NULL 값이 들어온 경우, NULL 반환
	EXPECT_NULL(CopyString(NULL, srcStr));
	EXPECT_NULL(CopyString(dstStr, NULL));
	EXPECT_NULL(CopyString(NULL, NULL));

    DeleteString(&srcStr);
    DeleteString(&dstStr);
})

TEST(CopyString, CopyNString, {
	char *srcData = "abcdef";
	char *dstData = "xyz";
	char *expected = "abc"; 
    StringPtr srcStr = NewString(srcData);
	StringPtr dstStr = NewString(dstData);

	// 반환값 확인
	EXPECT_NOT_NULL(CopyNString(dstStr, srcStr, 3));
	
	// destination 문자열의 길이와 source 문자열의 길이가 같은 경우
	dstData = "xyz";
	SetString(dstStr, dstData);
	EXPECT_STR_EQUAL(CopyNString(dstStr, srcStr, 3), expected);

	// destination 문자열의 길이가 복사할 문자열의 길이보다 큰 경우 정상 동작해야 한다.
	dstData = "defghi";
	SetString(dstStr, dstData);
	EXPECT_STR_EQUAL(CopyNString(dstStr, srcStr, 3), expected);

	// destination 문자열의 길이가 복사할 문자열의 길이보다 작은 경우에도 정상 동작해야 한다.
	dstData = "d";
	SetString(dstStr, dstData);
	EXPECT_STR_EQUAL(CopyNString(dstStr, srcStr, 3), expected);

	// 빈 문자열을 정상적으로 복사해야 한다.
	srcData = "";
	expected = "";
    SetString(srcStr, srcData);
	EXPECT_STR_EQUAL(CopyNString(dstStr, srcStr, 1), expected);

	// 복사할 문자열의 길이가 0 일 경우 NULL 을 반환
	EXPECT_NULL(CopyNString(dstStr, srcStr, 0));

	// NULL 값이 들어온 경우, NULL 반환
	EXPECT_NULL(CopyNString(NULL, srcStr, 1));
	EXPECT_NULL(CopyNString(dstStr, NULL, 1));
	EXPECT_NULL(CopyNString(NULL, NULL, 1));

	DeleteString(&srcStr);
    DeleteString(&dstStr);
})

TEST(FormatString, FormatString, {
	char *s = "abc";
	char *format = "[%s]";
	char expected[5];
	sprintf(expected, format, s);
	StringPtr str = NewString(s);

	// 반환값 확인
	EXPECT_NOT_NULL(FormatString(str, format, s));

	// 기본적인 함수 동작 테스트
	EXPECT_STR_EQUAL(FormatString(str, format, s), expected);

	// NULL 값이 들어온 경우, NULL 반환
	EXPECT_NULL(FormatString(NULL, format, s));
	EXPECT_NULL(FormatString(str, NULL, s));
	EXPECT_NULL(FormatString(str, format, NULL));
	EXPECT_NULL(FormatString(str, NULL, NULL));
	EXPECT_NULL(FormatString(NULL, NULL, s));
	EXPECT_NULL(FormatString(NULL, format, NULL));
	EXPECT_NULL(FormatString(NULL, NULL, NULL));

	DeleteString(&str);
})

TEST(ConcatString, ConcatString, {
	char *s1 = "abc";
	char *s2 = "def";
	char *expected = "abcdef";
	StringPtr str = NewString(s1);
	
	// 반환값 확인
	EXPECT_NOT_NULL(ConcatString(str, s2));

	// 함수 기본 동작 테스트
	EXPECT_STR_EQUAL(GetPtr(str), expected);

	// 원본 문자열에 빈문자열을 붙이는 경우, 원본 문자열 반환
	EXPECT_STR_EQUAL(ConcatString(str, ""), expected);

	// 빈문자열에 붙이는 경우, 붙이려는 문자열 반환
	SetString(str, "");
	EXPECT_STR_EQUAL(ConcatString(str, s2), s2);

	// 붙이려는 문자열이 NULL 인 경우, 원본 문자열을 반환
	EXPECT_NOT_NULL(ConcatString(str, NULL));
	// 원본 문자열 관리 구조체가 NULL 인 경우, NULL 반환
	EXPECT_NULL(ConcatString(NULL, s2));
	EXPECT_NULL(ConcatString(NULL, NULL));

	DeleteString(&str);
})

TEST(TruncateString, TruncateString, {
	char *s = "abcdef";
	char *expected = "abc";
	int from = 3; // zero based
	StringPtr str = NewString(s);

	// 반환값 확인
	EXPECT_NOT_NULL(TruncateString(str, from));

	// 함수 기본 동작 테스트
	EXPECT_STR_EQUAL(GetPtr(str), expected);

	// from 이 움수인 경우, NULL 을 반환
	from = -1;
	EXPECT_NULL(TruncateString(str, from));

	// from 이 문자열의 길이 보다 큰 경우, NULL 을 반환
	from = (int)strlen(s);
	EXPECT_NULL(TruncateString(str, from));

	// from 이 0 인 경우, 빈 문자열을 반환
	from = 0;
	EXPECT_STR_EQUAL(TruncateString(str, from), "");

	// 빈문자열을 자를 경우, NULL 을 반환
	SetString(str, "");
	EXPECT_NULL(TruncateString(str, from));

	// NULL 값이 들어온 경우, NULL 반환
	EXPECT_NULL(TruncateString(NULL, from));

	DeleteString(&str);
})

TEST(SubString, SubString, {
	char *s = "abcdef";
	StringPtr str = NewString(s);
	int from = 1;
	int length = 2;

	// 반환값 확인
	StringPtr actual = SubString(str, from, length);
	EXPECT_NOT_NULL(actual); // bc

	// 함수 기본 동작 테스트
	char *expected = "bc";
	EXPECT_STR_EQUAL(actual->data, expected);
	DeleteString(&actual);

	// from 테스트
	// from 이 음수인 경우
	actual = SubString(str, -1, length);
	EXPECT_NULL(actual);
	// from 이 문자열 길이보다 큰 경우
	actual = SubString(str, (int)strlen(str->data), length);
	EXPECT_NULL(actual);

	// length 테스트
	// length 가 음수인 경우
	actual = SubString(str, from, -1);
	EXPECT_NULL(actual);
	// from + length 가 문자열의 길이보다 큰 경우, from 부터 문자열의 마지막까지 추출해서 반환
	from = 3;
	actual = SubString(str, from, 10); // str->data : abcdef
	EXPECT_STR_EQUAL(actual->data, "def");
	DeleteString(&actual);
	// @@ length 가 0 인 경우, 빈 문자열을 반환
	actual = SubString(str, from, 0);
	EXPECT_STR_EQUAL(actual->data, "");
	DeleteString(&actual);

	// 빈문자열 테스트
	// from 이 원본 문자열 길이보다 큰 경우, NULL 반환
	SetString(str, "");
	actual = SubString(str, 1, 2);
	EXPECT_NULL(actual);

	// 빈문자열에서 from 이 0, length 가 0 인 경우, NULL 반환
	SetString(str, "");
	actual = SubString(str, 0, 0);
	EXPECT_NULL(actual);

	// NULL 값이 들어온 경우, NULL 반환
	actual = SubString(NULL, from, length);
	EXPECT_NULL(actual);

	DeleteString(&str);
})

TEST(CompareString, CompareString, {
	char *s1 = "abc";
	char *s2 = "abc";
	char *s3 = "abf";
	char *s4 = "abcf";
	char *s5 = "a12";
	char *s6 = "123";
	char *s7 = "*1*";
	char *s8 = "a*1*d";
	char *s9 = "bc";
	StringPtr str1 = NewString(s1);
	StringPtr str2 = NewString(s2);

	// 반환값 확인 및 기본 함수 동작 테스트
	// 길이가 서로 같은 두 개의 문자열이 같은 경우
	EXPECT_NUM_EQUAL(CompareString(str1, str2), 0);

	// 길이가 서로 같은 두 개의 문자열이 다른 경우
	// 비교할 문자열이 더 작은 경우
	// - abc < abf
	SetString(str1, s1);
	SetString(str2, s3);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), -1);
	// - 123 < a12
	SetString(str1, s6);
	SetString(str2, s5);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), -1);
	// - *1* < abf
	SetString(str1, s7);
	SetString(str2, s3);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), -1);

	// 비교할 문자열이 더 큰 경우
	// - abf > abc
	SetString(str1, s3);
	SetString(str2, s1);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), 1);
	// - a12 > 123
	SetString(str1, s5);
	SetString(str2, s6);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), 1);
	// - abf > *1*
	SetString(str1, s3);
	SetString(str2, s7);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), 1);

	// 길이가 서로 다른 두 개의 문자열이 다른 경우
	// 비교할 문자열이 더 작은 경우
	// - abc < abcf
	SetString(str1, s1);
	SetString(str2, s4);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), -1);
	// - a*1*d < abcf
	SetString(str1, s8);
	SetString(str2, s4);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), -1);
	// abcf < bc
	SetString(str1, s4);
	SetString(str2, s9);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), -1);

	// 비교할 문자열이 더 큰 경우
	// - abcf > abc
	SetString(str1, s4);
	SetString(str2, s1);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), 1);
	// abcf > a*1*d
	SetString(str1, s4);
	SetString(str2, s8);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), 1);
	// bc > abcf
	SetString(str1, s9);
	SetString(str2, s4);
	EXPECT_NUM_EQUAL(CompareString(str1, str2), 1);

	// 비교될 문자열이 빈문자열인 경우, -1 반환
	SetString(str1, "");
	EXPECT_NUM_EQUAL(CompareString(str1, str2), -1);
	// 비교할 문자열이 빈문자열인 경우, 1 반환
	SetString(str1, s1);
	SetString(str2, "");
	EXPECT_NUM_EQUAL(CompareString(str1, str2), 1);
	SetString(str1, "");
	EXPECT_NUM_EQUAL(CompareString(str1, str2), 0);
	
	// NULL 값이 들어온 경우, COMPARE_ERROR 반환
	SetString(str2, s2);
	EXPECT_NUM_EQUAL(CompareString(NULL, str2), COMPARE_ERROR);
	EXPECT_NUM_EQUAL(CompareString(str1, NULL), COMPARE_ERROR);
	EXPECT_NUM_EQUAL(CompareString(NULL, NULL), COMPARE_ERROR);

	DeleteString(&str1);
	DeleteString(&str2);
})

TEST(SearchString, SearchString, {
	char *s1 = "abcaabcbcabcdede";
	char *s2 = "b";
	char *s3 = "cd";
	char *s4 = "bcd";
	char *s5 = "bcde";
	char *s6 = "abcde";
	char *s7 = "abcdef";
	StringPtr str = NewString(s1);

	// 반환값 확인 및 기본 함수 동작 테스트
	// 검색할 문자열과 검색될 문자열이 일치하는 경우, True 를 반환
	// 검색할 문자열의 길이가 1
	EXPECT_NUM_EQUAL(SearchString(str, s2), True);
	// 검색할 문자열의 길이가 2
	EXPECT_NUM_EQUAL(SearchString(str, s3), True);
	// 검색할 문자열의 길이가 3
	EXPECT_NUM_EQUAL(SearchString(str, s4), True);
	// 검색할 문자열의 길이가 4
	EXPECT_NUM_EQUAL(SearchString(str, s5), True);              
	// 검색할 문자열의 길이가 5
	EXPECT_NUM_EQUAL(SearchString(str, s6), True);

	// 검색할 문자열과 검색될 문자열이 일치하지 않는 경우, False 를 반환
	// 검색할 문자열의 길이가 6 (검색할 문자열의 길이가 5)
	EXPECT_NUM_EQUAL(SearchString(str, s7), False);
	// 검색할 문자열이 포함되지 않는 문자열인 경우
	s7 = "z";
	EXPECT_NUM_EQUAL(SearchString(str, s7), False);
	// 검색할 문자열이 빈문자열인 경우
	s7 = "";
	EXPECT_NUM_EQUAL(SearchString(str, s7), False);
	// 검색될 문자열이 빈문자열인 경우
	SetString(str, "");
	EXPECT_NUM_EQUAL(SearchString(str, s2), SEARCH_ERROR);

	// NULL 값이 들어온 경우, False 반환
	EXPECT_NUM_EQUAL(SearchString(NULL, s2), SEARCH_ERROR);
	EXPECT_NUM_EQUAL(SearchString(str, NULL), SEARCH_ERROR);
	EXPECT_NUM_EQUAL(SearchString(NULL, NULL), SEARCH_ERROR);

	DeleteString(&str);
})

TEST(CheckCharIsDigit, IsDigit, {
	// 정상 케이스
	// 숫자 문자가 입력일 경우
	EXPECT_NUM_EQUAL(IsDigit('0'), True);
	EXPECT_NUM_EQUAL(IsDigit('4'), True);
	EXPECT_NUM_EQUAL(IsDigit('9'), True);

	// 비정상 케이스
	// 숫자 문자가 아닌 문자가 입력일 경우
	EXPECT_NUM_EQUAL(IsDigit('a'), False);
	EXPECT_NUM_EQUAL(IsDigit('*'), False);
	EXPECT_NUM_EQUAL(IsDigit('A'), False);
	// Whitespace 문자가 입력일 경우
	EXPECT_NUM_EQUAL(IsDigit(' '), False);
	EXPECT_NUM_EQUAL(IsDigit('\t'), False);
})

TEST(CheckCharIsAlpha, IsAlpha, {
	// 정상 케이스
	// 알파벳 문자가 입력일 경우
	EXPECT_NUM_EQUAL(IsAlpha('a'), True);
	EXPECT_NUM_EQUAL(IsAlpha('q'), True);
	EXPECT_NUM_EQUAL(IsAlpha('z'), True);
	EXPECT_NUM_EQUAL(IsAlpha('A'), True);
	EXPECT_NUM_EQUAL(IsAlpha('S'), True);
	EXPECT_NUM_EQUAL(IsAlpha('Z'), True);

	// 비정상 케이스
	// 알파벳 문자가 아닌 문자가 입력일 경우
	EXPECT_NUM_EQUAL(IsAlpha('1'), False);
	EXPECT_NUM_EQUAL(IsAlpha('*'), False);
	EXPECT_NUM_EQUAL(IsAlpha('0'), False);
	// Whitespace 문자가 입력일 경우
	EXPECT_NUM_EQUAL(IsAlpha(' '), False);
	EXPECT_NUM_EQUAL(IsAlpha('\t'), False);
})

TEST(CheckCharIsLetter, IsLetter, {
	EXPECT_NUM_EQUAL(IsLetter('a'), True);
	EXPECT_NUM_EQUAL(IsLetter('_'), True);
	EXPECT_NUM_EQUAL(IsLetter('Z'), True);
	EXPECT_NUM_EQUAL(IsLetter('q'), True);

	EXPECT_NUM_EQUAL(IsLetter('1'), False);
	EXPECT_NUM_EQUAL(IsLetter('-'), False);
})

TEST(CheckCharIsSpace, IsSpace, {
	EXPECT_NUM_EQUAL(IsSpace(' '), True);
	EXPECT_NUM_EQUAL(IsSpace('\t'), True);

	EXPECT_NUM_EQUAL(IsSpace('a'), False);
	EXPECT_NUM_EQUAL(IsSpace('\n'), False);
})

TEST(CheckCharIsCRLF, IsCRLF, {
	EXPECT_NUM_EQUAL(IsCRLF('\r'), True);
	EXPECT_NUM_EQUAL(IsCRLF('\n'), True);

	EXPECT_NUM_EQUAL(IsCRLF(' '), False);
	EXPECT_NUM_EQUAL(IsCRLF('a'), False);
})

TEST(SplitString, SplitString, {
	char *s = "ab|zf|de";
	char delimiter = '|';
	char *expected1 = "ab";
	char *expected2 = "zf";
	char *expected3 = "de";
	char **actual = SplitString(s, delimiter, IncludeEmptyArray);

	// 반환값 확인 
	EXPECT_NOT_NULL(actual);

	// 함수 기본 정상 동작 확인
	EXPECT_STR_EQUAL(actual[0], expected1);
	EXPECT_STR_EQUAL(actual[1], expected2);
	EXPECT_STR_EQUAL(actual[2], expected3);
	EXPECT_NULL(actual[3]);
	char **tempActual = actual;
	while(*tempActual != NULL)
	{
		//printf("%s\n", *tempActual);
		free(*tempActual);
		tempActual++;
	}
	free(actual);

	delimiter = 'a';
	actual = SplitString(s, delimiter, IncludeEmptyArray);
	expected1 = "";
	expected2 = "b|zf|de";
	EXPECT_STR_EQUAL(actual[0], expected1);
	EXPECT_STR_EQUAL(actual[1], expected2);
	EXPECT_NULL(actual[2]);
	tempActual = actual;
	while(*tempActual != NULL)
	{
		//printf("%s\n", *tempActual);
		free(*tempActual);
		tempActual++;
	}
	free(actual);

	delimiter = 'a';
	actual = SplitString(s, delimiter, ExcludeEmptyArray);
	expected1 = "b|zf|de";
	EXPECT_STR_EQUAL(actual[0], expected1);
	EXPECT_NULL(actual[1]);
	tempActual = actual;
	while(*tempActual != NULL)
	{
		//printf("%s\n", *tempActual);
		free(*tempActual);
		tempActual++;
	}
	free(actual);

	delimiter = '\n';
	s = "1a b 2c\n#ef\n\t@12#tg3\ndg\t2 e!";
	actual = SplitString(s, delimiter, IncludeEmptyArray);
	expected1 = "1a b 2c";
	expected2 = "#ef";
	expected3 = "\t@12#tg3";
	char *expected4 = "dg\t2 e!";
	EXPECT_STR_EQUAL(actual[0], expected1);
	EXPECT_STR_EQUAL(actual[1], expected2);
	EXPECT_STR_EQUAL(actual[2], expected3);
	EXPECT_STR_EQUAL(actual[3], expected4);
	EXPECT_NULL(actual[4]);
	tempActual = actual;
	while(*tempActual != NULL)
	{
		//printf("%s\n", *tempActual);
		free(*tempActual);
		tempActual++;
	}
	free(actual);

	// delimeter 가 널 문자이면, 원본 문자열 그대로 복사해서 반환
	delimiter = '\0';
	s = "ab|de|wf";
	expected1 = s;
	// delimiter 가 널 문자인 경우, option 값은 무의미 > 널 문자는 문자열에 미포함
	actual = SplitString(s, delimiter, IncludeEmptyArray);
	EXPECT_STR_EQUAL(actual[0], expected1);
	EXPECT_NULL(actual[1]);
	tempActual = actual;
	while(*tempActual != NULL)
	{
		//printf("%s\n", *tempActual);
		free(*tempActual);
		tempActual++;
	}
	free(actual);

	// 비정상 동작
	// 빈문자열이 들어온 경우, NULL 반환
	EXPECT_NULL(SplitString("", 'x', IncludeEmptyArray));
	// 잘못된 delimiter
	EXPECT_NULL(SplitString(s, 'x', IncludeEmptyArray));
	// NULL 값이 들어온 경우, NULL 반환
	EXPECT_NULL(SplitString(NULL, 'x', IncludeEmptyArray));
})

TEST(MergeString, MergeString, {
	char *s = "ab de wf";
	char *expected = "abdewf";
	char delimiter = ' ';
	char *actual = MergeString(s, delimiter);

	// 반환값 확인
	EXPECT_NOT_NULL(actual);

	// 정상 동작
	EXPECT_STR_EQUAL(actual, expected);
	free(actual);

	s = "a1 2d\n12\t@* @9\n\ncvw e ag\n\n\n4g h";
	delimiter = '\n';
	expected = "a1 2d12\t@* @9cvw e ag4g h";
	actual = MergeString(s, delimiter);
	EXPECT_STR_EQUAL(actual, expected);
	free(actual);

	// delimeter 가 널 문자이면, 원본 문자열 그대로 복사해서 반환
	delimiter = '\0';
	s = "ab de wf";
	expected = s;
	actual = MergeString(s, delimiter);
	EXPECT_STR_EQUAL(actual, expected);
	free(actual);

	// 비정상 동작
	// 빈문자열이 들어온 경우, NULL 반환
	EXPECT_NULL(MergeString("", delimiter));
	// 잘못된 delimiter
	EXPECT_NULL(MergeString(s, 'x'));
	// NULL 값이 들어온 경우, NULL 반환
	EXPECT_NULL(MergeString(NULL, 'x'));
})

int main()
{
    CREATE_TESTSUIT();

    REGISTER_TESTS(
        Test_NewString_InstanceCreation,
        Test_DeleteString_InstanceDeletion,
        Test_GetLength_LengthOfString,
        Test_GetPtr_PointerToStringData,
        Test_CloneString_InstanceClone,
        Test_SetString_SetNewValue,
		Test_ConvertToUpperCase_UpperString,
		Test_ConvertToLowerCase_LowerString,
		Test_TrimString_RemoveLeftSpace,
		Test_TrimString_RemoveRightSpace,
		Test_TrimString_RemoveBothSpace,
		Test_CopyString_CopyString,
		Test_CopyString_CopyNString,
		Test_FormatString_FormatString,
		Test_ConcatString_ConcatString,
		Test_TruncateString_TruncateString,
		Test_SubString_SubString,
		Test_CompareString_CompareString,
		Test_SearchString_SearchString,
		Test_CheckCharIsDigit_IsDigit,
		Test_CheckCharIsAlpha_IsAlpha,
		Test_CheckCharIsLetter_IsLetter,
		Test_CheckCharIsSpace_IsSpace,
		Test_CheckCharIsCRLF_IsCRLF,
		Test_SplitString_SplitString,
		Test_MergeString_MergeString
    );

    RUN_ALL_TESTS();

    CLEAN_UP_TESTSUIT();

	return 1;
}

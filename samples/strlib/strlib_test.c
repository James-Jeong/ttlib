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

    // NULL 값이 들어온 경우, 0을 반환해야 한다.
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

    // 빈 문자열에 대해서, 0을 반환해야 한다.
    s = "";
    str = NewString(s);
    expected = 0;
    EXPECT_NUM_EQUAL(GetLength(str), expected);
    DeleteString(&str);

    // NULL 값이 들어온 경우, 0을 반환해야 한다.
    expected = 0;
    EXPECT_NUM_EQUAL(GetLength(NULL), expected);
})

TEST(GetPtr, PointerToStringData, {
    char *s = "abcd";
    StringPtr str = NewString(s);

    EXPECT_PTR_EQUAL(GetPtr(str), str->data);
    DeleteString(&str);

    // NULL 값이 들어온 경우, NULL을 반환해야 한다.
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

	// NULL 값이 들어온 경우, NULL을 반환해야 한다.
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

	// NULL 값이 들어온 경우, NULL을 반환해야 한다.
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

	// NULL 값이 들어온 경우, NULL을 반환해야 한다.
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

	// NULL 값이 들어온 경우, NULL을 반환해야 한다.
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

	// NULL 값이 들어온 경우, NULL을 반환해야 한다.
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

	// NULL 값이 들어온 경우, NULL을 반환해야 한다.
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

	// NULL 값이 들어온 경우, NULL을 반환해야 한다.
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

	// NULL 값이 들어온 경우, NULL을 반환해야 한다.
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

	// 복사할 문자열의 길이가 0 일 경우 NULL 을 반환해야 한다.
	EXPECT_NULL(CopyNString(dstStr, srcStr, 0));

	// NULL 값이 들어온 경우, NULL을 반환해야 한다.
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

	DeleteString(&str);
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
		Test_FormatString_FormatString
    );

    RUN_ALL_TESTS();

    CLEAN_UP_TESTSUIT();

    return 1;
}

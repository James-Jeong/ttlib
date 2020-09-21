#include "ttlib.h"
#include "strlib.h"

DECLARE_TEST();

TEST(NewString, InstanceCreation, {
    char *s = "my string";
    char *expected = s;
    StringPtr str = NewString(s);
    EXPECT_NOT_NULL(str);
    EXPECT_STR_EQUAL(str->data, expected);    
    DeleteString(&str);

    str = NewString(NULL);
    EXPECT_NULL(str);
})

TEST(DeleteString, InstanceDeletion, {
    StringPtr str = NewString("abcd");
    DeleteString(&str);
    EXPECT_NULL(str);
})

TEST(GetLength, LengthOfString, {
    // 정상적으로 문자열 개수를 반환해야 한다.
    char *s = "abcd";
    StringPtr str = NewString(s);
    size_t expected = strlen(s);

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

    EXPECT_NUM_EQUAL(GetPtr(str), str->data);
    DeleteString(&str);

    // NULL 값이 들어온 경우, NULL을 반환해야 한다.
    EXPECT_NULL(GetPtr(NULL));

})

TEST(CloneString, InstanceClone, {
    char *s = "abcd";
    StringPtr str1 = NewString(s);
    StringPtr str2 = CloneString(str1);

    EXPECT_NOT_NULL(str2);
    EXPECT_STR_EQUAL(str1->data, str2->data);
    EXPECT_NUM_NOT_EQUAL(str1->data, str2->data);
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
    StringPtr cloned = CloneString(str);

    EXPECT_STR_EQUAL(str->data, cloned->data);

    EXPECT_STR_EQUAL(SetString(str, expected), expected);

	// NULL 값이 들어온 경우, NULL을 반환해야 한다.
    EXPECT_NULL(SetString(str, NULL));
    EXPECT_NULL(SetString(NULL, expected));

    // 빈 문자열을 정상처리해야 한다.
    expected = "";
    EXPECT_STR_EQUAL(SetString(str, expected), expected);
    EXPECT_NUM_EQUAL(GetLength(str), strlen(expected));

    DeleteString(&str);
})

TEST(ConvertToUpperCase, UpperString, {
	char *s = "abcd";
	char *expected = "ABCD"; 
    StringPtr str = NewString(s);

	// 반환 값 확인
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

	EXPECT_NOT_NULL(ConvertToLowerCase(str));

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

	EXPECT_NOT_NULL(RemoveLeftSpace(str));

	// 왼쪽 공백만 있는 경우
	s = " \t\t \tabcde";
    SetString(str, s);
	//printf("Before) expected:%s, str->data:%s\n", expected, str->data);
	EXPECT_STR_EQUAL(RemoveLeftSpace(str), expected);
	//printf("After) expected:%s, str->data:%s\n", expected, str->data);

	// 왼쪽, 오른쪽 공백과 문자열 중간에 공백이 있는 경우
	s = " \ta b c d e   ";
	expected = "a b c d e   ";
    SetString(str, s);
	//printf("Before) expected:%s, str->data:%s|\n", expected, str->data);
	EXPECT_STR_EQUAL(RemoveLeftSpace(str), expected);
	//printf("After) expected:%s, str->data:%s|\n", expected, str->data);

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

	EXPECT_NOT_NULL(RemoveRightSpace(str));

	// 오른쪽 공백만 있는 경우
	s = "abcde \t\t \t";
    SetString(str, s);
	//printf("Before) expected:%s, str->data:%s|\n", expected, str->data);
	EXPECT_STR_EQUAL(RemoveRightSpace(str), expected);
	//printf("After) expected:%s, str->data:%s|\n", expected, str->data);

	s = " a b c d e \t\t \t";
	expected = " a b c d e";
    SetString(str, s);
	//printf("Before) expected:%s, str->data:%s|\n", expected, str->data);
	EXPECT_STR_EQUAL(RemoveRightSpace(str), expected);
	//printf("After) expected:%s, str->data:%s|\n", expected, str->data);

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

	EXPECT_NOT_NULL(RemoveBothSpace(str));

	s = "\t \t\t abcde \t\t \t";
     SetString(str, s);
	//printf("Before) expected:%s, str->data:%s|\n", expected, str->data);
	EXPECT_STR_EQUAL(RemoveBothSpace(str), expected);
	//printf("After) expected:%s, str->data:%s|\n", expected, str->data);

	s = "\t \t\t a b c d e \t\t \t";
	expected = "a b c d e";
	SetString(str, s);
	//printf("Before) expected:%s, str->data:%s|\n", expected, str->data);
	EXPECT_STR_EQUAL(RemoveBothSpace(str), expected);
	//printf("After) expected:%s, str->data:%s|\n", expected, str->data);

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

	EXPECT_NOT_NULL(CopyString(dstStr, srcStr));

	dstData = "def";
	SetString(dstStr, dstData);
	//printf("Before) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);
	EXPECT_STR_EQUAL(CopyString(dstStr, srcStr), expected);
	//printf("After) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);

	// destination 문자열의 길이가 source 문자열의 길이보다 큰 경우 정상 동작해야 한다.
	dstData = "defghi";
	SetString(dstStr, dstData);
	//printf("Before) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);
	EXPECT_STR_EQUAL(CopyString(dstStr, srcStr), expected);
	//printf("After) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);

	// destination 문자열의 길이가 source 문자열의 길이보다 작은 경우에도 정상 동작해야 한다.
	dstData = "d";
	SetString(dstStr, dstData);
	//printf("Before) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);
	EXPECT_STR_EQUAL(CopyString(dstStr, srcStr), expected);
	//printf("After) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);

	// 빈 문자열을 정상적으로 복사해야 한다.
	srcData = "";
	expected = "";
    SetString(srcStr, srcData);
	//printf("Before) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);
	EXPECT_STR_EQUAL(CopyString(dstStr, srcStr), expected);
	//printf("After) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);

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

	EXPECT_NOT_NULL(CopyNString(dstStr, srcStr, 3));
	
	dstData = "xyz";
	SetString(dstStr, dstData);
	//printf("Before) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);
	EXPECT_STR_EQUAL(CopyNString(dstStr, srcStr, 3), expected);
	//printf("After) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);

	// destination 문자열의 길이가 복사할 문자열의 길이보다 큰 경우 정상 동작해야 한다.
	dstData = "defghi";
	SetString(dstStr, dstData);
	//printf("Before) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);
	EXPECT_STR_EQUAL(CopyNString(dstStr, srcStr, 3), expected);
	//printf("After) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);

	// destination 문자열의 길이가 복사할 문자열의 길이보다 작은 경우에도 정상 동작해야 한다.
	dstData = "d";
	SetString(dstStr, dstData);
	//printf("Before) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);
	EXPECT_STR_EQUAL(CopyNString(dstStr, srcStr, 3), expected);
	//printf("After) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);

	// 빈 문자열을 정상적으로 복사해야 한다.
	srcData = "";
	expected = "";
    SetString(srcStr, srcData);
	//printf("Before) expected:%s, srcStr->data:%s(%d), dstStr->data:%s(%d)|\n", expected, srcStr->data, srcStr->length, dstStr->data, dstStr->length);
	EXPECT_STR_EQUAL(CopyNString(dstStr, srcStr, 1), expected);
	//printf("After) expected:%s, srcStr->data:%s, dstStr->data:%s|\n", expected, srcStr->data, dstStr->data);

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
	StringPtr str = NewString(s);

	char expected[5];
	sprintf(expected, format, s);

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

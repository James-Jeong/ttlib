#include "ttlib.h"
#include "strlib.h"

DECLARE_TEST()

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
    int expected = strlen(s);

    EXPECT_NUM_EQUAL(GetLength(str), expected);
    DeleteString(&str);

    // 빈 문자열에 대해서, 0을 반환해야 한다.
    s = "";
    str = NewString(s);
    expected = 0;
    EXPECT_NUM_EQUAL(GetLength(str), expected);
    DeleteString(&str);

    // NULL 값이 들어온 경우, -1을 반환해야 한다.
    expected = -1;
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
    EXPECT_NULL(SetString(str, NULL));
    EXPECT_NULL(SetString(NULL, expected));

    expected = "";
    EXPECT_STR_EQUAL(SetString(str, expected), expected);
    EXPECT_NUM_EQUAL(GetLength(str), strlen(expected));

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
        Test_SetString_SetNewValue
    );

    RUN_ALL_TESTS();

    CLEAN_UP_TESTSUIT();

    return 1;
}

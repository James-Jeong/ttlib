#include <stdio.h>
#include <stdlib.h>

#include "ttlib.h"
#include "mylib.h"

DECLARE_TEST()

///////////////////////////////////////////////////////////////////////////////
// Test Codes comes here -->

//////////////////////////////////////////////////////////////////////////////////
/// Number Tests
//////////////////////////////////////////////////////////////////////////////////

TEST(IsDigit, AcceptableValues, {
    EXPECT_NUM_EQUAL(IsDigit('0'), True, int);
    EXPECT_NUM_EQUAL(IsDigit('1'), True, int);
    EXPECT_NUM_EQUAL(IsDigit('2'), True, int);
    EXPECT_NUM_EQUAL(IsDigit('3'), True, int);
    EXPECT_NUM_EQUAL(IsDigit('4'), True, int);
    //ASSERT_NUM_EQUAL(IsDigit('5'), False, int);
    EXPECT_NUM_EQUAL(IsDigit('5'), False, int);
    EXPECT_NUM_EQUAL(IsDigit('6'), True, int);
    EXPECT_NUM_EQUAL(IsDigit('7'), True, int);
    EXPECT_NUM_EQUAL(IsDigit('8'), True, int);
    EXPECT_NUM_EQUAL(IsDigit('9'), True, int);
})

TEST(IsDigit, InvalidValues, {
    EXPECT_NUM_NOT_EQUAL(8, 8, int);
    EXPECT_NUM_NOT_EQUAL(IsDigit('A'), True, int);
    EXPECT_NUM_NOT_EQUAL(IsDigit('Z'), True, int);
    EXPECT_NUM_NOT_EQUAL(IsDigit('a'), True, int);
    EXPECT_NUM_NOT_EQUAL(IsDigit('z'), True, int);
    EXPECT_NUM_NOT_EQUAL(IsDigit('~'), True, int);
    EXPECT_NUM_NOT_EQUAL(IsDigit('+'), True, int);
})

TEST(CompareNumbers, Equal, {
    EXPECT_NUM_EQUAL(1, 1, int);
    EXPECT_NUM_EQUAL(1, 2, int);
})

TEST(CompareNumbers, NotEqual, {
    EXPECT_NUM_NOT_EQUAL(1, 1, int);
    EXPECT_NUM_NOT_EQUAL(1, 2, int);
})

TEST(CompareNumbers, Less, {
    EXPECT_NUM_LESS_EQUAL(1, 1, int);
    EXPECT_NUM_LESS_EQUAL(1, 2, int);
    EXPECT_NUM_LESS_EQUAL(2, 1, int);
    EXPECT_NUM_LESS_THAN(1, 1, int);
    EXPECT_NUM_LESS_THAN(1, 2, int);
    EXPECT_NUM_LESS_THAN(2, 1, int);
})

TEST(CompareNumbers, Greater, {
    EXPECT_NUM_GREATER_EQUAL(1, 1, int);
    EXPECT_NUM_GREATER_EQUAL(1, 2, int);
    EXPECT_NUM_GREATER_EQUAL(2, 1, int);
    EXPECT_NUM_GREATER_THAN(1, 1, int);
	EXPECT_NUM_GREATER_THAN(1, 2, int);
    EXPECT_NUM_GREATER_THAN(2, 1, int);
})

TEST(CheckNumberType, Even, {
    EXPECT_NUM_EVEN(1234, int);
    EXPECT_NUM_EVEN(2345, int);
})

TEST(CheckNumberType, Odd, {
    EXPECT_NUM_ODD(1234, int);
    EXPECT_NUM_ODD(2345, int);
})

//////////////////////////////////////////////////////////////////////////////////
/// String Tests
//////////////////////////////////////////////////////////////////////////////////

TEST(CompareStrings, Equal, {
    EXPECT_STR_EQUAL("abc", "ab");
    EXPECT_STR_EQUAL("abc", "abc");
})

TEST(CompareStrings, NotEqual, {
    EXPECT_STR_NOT_EQUAL("abc", "ab");
    EXPECT_STR_NOT_EQUAL("abc", "abc");
})

//<-- Test Codes ends here
///////////////////////////////////////////////////////////////////////////////

int main()
{
    CREATE_TESTSUIT();

    REGISTER_TESTS(
        Test_IsDigit_AcceptableValues,
        Test_IsDigit_InvalidValues,
        Test_CompareNumbers_Equal,
        Test_CompareNumbers_NotEqual,
        Test_CompareNumbers_Less,
        Test_CompareNumbers_Greater,
        Test_CheckNumberType_Even,
        Test_CheckNumberType_Odd,
        Test_CompareStrings_Equal,
        Test_CompareStrings_NotEqual);

    RUN_ALL_TESTS();

    CLEAN_UP_TESTSUIT();

    return True;
}

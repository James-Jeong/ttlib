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
    EXPECT_NUM_EQUAL(IsDigit('0'), True);
    EXPECT_NUM_EQUAL(IsDigit('1'), True);
    EXPECT_NUM_EQUAL(IsDigit('2'), True);
    EXPECT_NUM_EQUAL(IsDigit('3'), True);
    EXPECT_NUM_EQUAL(IsDigit('4'), True);
    //ASSERT_NUM_EQUAL(IsDigit('5'), False);
    EXPECT_NUM_EQUAL(IsDigit('5'), False);
    EXPECT_NUM_EQUAL(IsDigit('6'), True);
    EXPECT_NUM_EQUAL(IsDigit('7'), True);
    EXPECT_NUM_EQUAL(IsDigit('8'), True);
    EXPECT_NUM_EQUAL(IsDigit('9'), True);
})

TEST(IsDigit, InvalidValues, {
    EXPECT_NUM_NOT_EQUAL(8, 8);
    EXPECT_NUM_NOT_EQUAL(IsDigit('A'), True);
    EXPECT_NUM_NOT_EQUAL(IsDigit('Z'), True);
    EXPECT_NUM_NOT_EQUAL(IsDigit('a'), True);
    EXPECT_NUM_NOT_EQUAL(IsDigit('z'), True);
    EXPECT_NUM_NOT_EQUAL(IsDigit('~'), True);
    EXPECT_NUM_NOT_EQUAL(IsDigit('+'), True);
})

TEST(CompareNumbers, Equal, {
	EXPECT_NUM_EQUAL(1, 1);
	EXPECT_NUM_EQUAL(1, 2);
})

TEST(CompareNumbers, NotEqual, {
	EXPECT_NUM_NOT_EQUAL(1, 1);
	EXPECT_NUM_NOT_EQUAL(1, 2);
})

TEST(CompareNumbers, Less, {
    EXPECT_NUM_LESS_EQUAL(1, 1);
    EXPECT_NUM_LESS_EQUAL(1, 2);
    EXPECT_NUM_LESS_EQUAL(2, 1);
    EXPECT_NUM_LESS_THAN(1, 1);
    EXPECT_NUM_LESS_THAN(1, 2);
    EXPECT_NUM_LESS_THAN(2, 1);
})

TEST(CompareNumbers, Greater, {
    EXPECT_NUM_GREATER_EQUAL(1, 1);
    EXPECT_NUM_GREATER_EQUAL(1, 2);
    EXPECT_NUM_GREATER_EQUAL(2, 1);
    EXPECT_NUM_GREATER_THAN(1, 1);
    EXPECT_NUM_GREATER_THAN(1, 2);
    EXPECT_NUM_GREATER_THAN(2, 1);
})

TEST(CheckNumberType, Even, {
    EXPECT_NUM_EVEN(1234);
    EXPECT_NUM_EVEN(2345);
})

TEST(CheckNumberType, Odd, {
    EXPECT_NUM_ODD(1234);
    EXPECT_NUM_ODD(2345);
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
		Test_CompareStrings_NotEqual
    );

    RUN_ALL_TESTS();

    CLEAN_UP_TESTSUIT();

	return True;
}

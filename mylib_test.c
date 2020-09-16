#include <stdio.h>
#include <stdlib.h>

#include "ttlib.h"
#include "mylib.h"

DECLARE_TEST()

///////////////////////////////////////////////////////////////////////////////
// Test Codes comes here -->

TEST(IsDigit, AcceptableValues, {
    EXPECT_NUM_EQUAL(IsDigit('0'), True);
    EXPECT_NUM_EQUAL(IsDigit('1'), True);
    EXPECT_NUM_EQUAL(IsDigit('2'), True);
    EXPECT_NUM_EQUAL(IsDigit('3'), True);
    EXPECT_NUM_EQUAL(IsDigit('4'), True);
    EXPECT_NUM_EQUAL(IsDigit('5'), True);
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

//<-- Test Codes ends here
///////////////////////////////////////////////////////////////////////////////

int main() 
{
    CREATE_TESTSUIT();

    REGISTER_TESTS(
        Test_IsDigit_AcceptableValues,
        Test_IsDigit_InvalidValues
    );

    RUN_ALL_TESTS();

    CLEAN_UP_TESTSUIT();
}

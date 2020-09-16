#include <stdio.h>
#include <stdlib.h>

#include "ttlib.h"
#include "mylib.h"

DECLARE_TEST()

///////////////////////////////////////////////////////////////////////////////
// Test Codes comes here -->

TEST(IsDigit, AcceptableValues, {
    printf("AcceptableValues CALLED!!\n\n");
})

TEST(IsDigit, InvalidValues, {
    printf("InvalidValues CALLED !!\n\n");
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

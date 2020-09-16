#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ttlib.h"

////////////////////////////////////////////////////////////////////////////////
/// Predefitions of Static Functions
//////////////////////////////////////////////////////////////////////////////////

static void initializeTests(TestSuitPtr testSuit);
static void deleteTest(TestPtr test);

////////////////////////////////////////////////////////////////////////////////
/// Local Functions
//////////////////////////////////////////////////////////////////////////////////

TestSuitPtr NewTestSuit()
{
    // Create a TestSuit instance
    TestSuitPtr testSuit = (TestSuitPtr)malloc(sizeof(TestSuit));
    if (testSuit == NULL)
    {
        return NULL;
    }

    // Initialize other members
    testSuit->initializers = NULL;
    testSuit->numberOfTests = 0;

    return testSuit;
}

void RegisterInitializers(TestSuitPtr testSuit, TestSuitInitializer **initializers)
{
    if (testSuit == NULL || initializers == NULL)
    {
        return;
    }

    TestSuitInitializer *initializer = NULL;
    int count = 0;
    while ((initializer = *initializers++) != NULL)
    {
        count++;
    }

    if (count > 0)
    {
        if (testSuit->initializers == NULL)
        {

        }
    }
}

TestPtr AddTest(TestSuitPtr testSuit, Test test)
{
    // Check parameter
    if (testSuit == NULL)
    {
        return NULL;
    }

    // Create a new Test instance
    TestPtr newTest = (TestPtr)malloc(sizeof(Test));
    if (newTest == NULL)
    {
        return NULL;
    }

    // Allocate required memory for the created instance
    char *testCase = strdup(test.testCase);
    char *testName = strdup(test.testName);
    if (testCase == NULL || testName == NULL)
    {
        free(testCase);
        free(testName);
        free(newTest);

        return NULL;
    }

    // Setup members of Test instance
    newTest->testCase = testCase;
    newTest->testName = testName;
    newTest->testFunc = test.testFunc;

    // Reallocate memory for tests in TestSuit instance
    int numberOfTests = testSuit->numberOfTests;
    size_t newSize = 0;
    TestPtr allocatedTests = NULL;
    if (testSuit->tests == NULL) // None of tests registered yet.
    {
        allocatedTests = (TestPtr)malloc(sizeof(Test));
    }
    else    // One or more tests exist already.
    {
        newSize = sizeof(Test) * (size_t)(numberOfTests + 1);
        allocatedTests = (TestPtr)realloc(testSuit->tests, newSize);
    }
    if (allocatedTests == NULL)
    {
        puts("realloc failed!");
        deleteTest(newTest);
        return NULL;
    }

    // Add new Test instance at the end of the TestSuit instance
    memcpy(allocatedTests + numberOfTests, newTest, sizeof(Test));

    // Apply results to any related members
    testSuit->tests = allocatedTests;
    testSuit->numberOfTests++;

    return newTest;
}

void DeleteTestSuit(TestSuitPtrContainer testSuitContainer)
{
    // Check parameter
    if (testSuitContainer == NULL || *testSuitContainer == NULL)
    {
        return;
    }

    TestSuitPtr testSuit = *testSuitContainer;
    // release memory allocated to the array of Test instances
    TestPtr test = NULL;
	int t = 0;
    for (; t < testSuit->numberOfTests; t++)
    {
        test = &testSuit->tests[t];
        free(test->testCase);
        free(test->testName);
    }
    // release memory allocated to the TestSuit instance
    free(testSuit);
    // set NULL to the container of pointer to TestSuit instance
    *testSuitContainer = NULL;
}

void RunAllTests(TestSuitPtr testSuit)
{
    // Add user tests into TestSuit instance
    initializeTests(testSuit);

    printf("총 테스트 수: %d 개\n", testSuit->numberOfTests);

	int t = 0;
    int count = testSuit->numberOfTests;
    if (count >= 1)
    {
        // Call all test functions in the array
        TestPtr test = NULL;
        for (t = 0; t < count; t++)
        {
            test = &testSuit->tests[t];
            printf("테스트 케이스: %s, 테스트: %s\n", test->testCase, test->testName);
            if (test->testFunc == NULL) // testSuit->tests is a NULL-terninated array
            {
                break;
            }
            test->testFunc(testSuit);
        }
        puts("calling tests done.");
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Static Functions
//////////////////////////////////////////////////////////////////////////////////

static void initializeTests(TestSuitPtr testSuit)
{
    TestSuitInitializer *initializers = testSuit->initializers;
    if (initializers == NULL)
    {
        return;
    }
    
    TestSuitInitializer initializer = NULL;
    while ((initializer = *initializers++) != NULL)
    {
        initializer();
    }
}

static void deleteTest(TestPtr test)
{
    if (test == NULL)
    {
        return;
    }
    free(test->testCase);
    free(test->testName);
    free(test);
}

////////////////////////////////////////////////////////////////////////////////
/// Util Functions
//////////////////////////////////////////////////////////////////////////////////

void print_message_helper(const char *file_name, int line_number, const char *msg, TEST_RESULT test_result_type)
{
	switch(test_result_type)
	{
		case SUCCESS:
			printf("[SUCCESS]\n");
			break;
		case FATAL_FAIL:
			printf("[FAIL] %s (file:%s, line:%d)\nTest aborted.\n", msg, file_name, line_number);
			break;
		case NON_FATAL_FAIL:
			printf("[FAIL] %s (file:%s, line:%d)\n", msg, file_name, line_number);
			break;
		default:
			printf("[UNKNOWN] %s (file:%s, line:%d)\n", msg, file_name, line_number);
			break;
	}	
}

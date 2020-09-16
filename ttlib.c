#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ttlib.h"

static void initializeTests(TestSuitPtr testSuit);
static void deleteTest(TestPtr test);

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
    // int lengthOfTestCase = strlen(test.testCase) + 1;
    // int lengthOfTestName = strlen(test.testName) + 1;
    // char *testCase = (char *)calloc(1, lengthOfTestCase);
    // char *testName = (char*)calloc(1, lengthOfTestName);
    if (testCase == NULL || testName == NULL)
    {
        free(testCase);
        free(testName);
        free(newTest);

        return NULL;
    }
    // memcpy(testCase, test.testCase, lengthOfTestCase);
    // memcpy(testName, test.testName, lengthOfTestName);
    
    // Setup members of Test instance
    newTest->testCase = testCase;
    newTest->testName = testName;
    newTest->testFunc = test.testFunc;

    // Reallocate memory for tests in TestSuit instance
    int numberOfTests = testSuit->numberOfTests;
    int newSize = 0;
    TestPtr allocatedTests = NULL;
    if (testSuit->tests == NULL) // None of tests registered yet.
    {
        allocatedTests = (TestPtr)malloc(sizeof(Test));
    }
    else    // One or more tests exist already.
    {
        newSize = sizeof(Test) * (numberOfTests + 1);
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
    for (int t = 0; t < testSuit->numberOfTests; t++)
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

    int count = testSuit->numberOfTests;
    if (count >= 1)
    {
        // Call all test functions in the array
        TestPtr test = NULL;
        for (int t = 0; t < count; t++)
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

void initializeTests(TestSuitPtr testSuit)
{
    TestSuitInitializer *initializers = testSuit->initializers;
    TestSuitInitializer initializer = NULL;
    while ((initializer = *initializers++) != NULL)
    {
        initializer();
    }
}

void deleteTest(TestPtr test)
{
    if (test == NULL)
    {
        return;
    }
    free(test->testCase);
    free(test->testName);
    free(test);
}

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

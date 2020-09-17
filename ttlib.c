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
	testSuit->failTests = NULL;
    testSuit->numberOfTests = 0;
	testSuit->numberOfSuccessTests = 0;
	testSuit->numberOfFailTests = 0;
	testSuit->totalNumOfSuccessTestFuncs = 0;
	testSuit->totalNumOfFailTestFuncs = 0;

    return testSuit;
}

void AddTest(TestSuitPtr testSuit, Test test)
{
    // Check parameter
    if (testSuit == NULL)
    {
        return;
    }

    // Create a new Test instance
    TestPtr newTest = (TestPtr)malloc(sizeof(Test));
    if (newTest == NULL)
    {
        return;
    }

    // Allocate required memory for the created instance
    char *testCase = strdup(test.testCase);
    char *testName = strdup(test.testName);
    if (testCase == NULL || testName == NULL)
    {
        free(testCase);
        free(testName);
        free(newTest);
        return;
    }

    // Setup members of Test instance
    newTest->testCase = testCase;
    newTest->testName = testName;
    newTest->testFunc = test.testFunc;
	newTest->numberOfSuccessTestFunc = 0;
	newTest->numberOfFailTestFunc = 0;

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
        puts("realloc failed! (allocatedTests)");
        deleteTest(newTest);
        return;
    }

    // Add new Test instance at the end of the TestSuit instance
    memcpy(allocatedTests + numberOfTests, newTest, sizeof(Test));

    // Apply results to any related members
    testSuit->tests = allocatedTests;
    testSuit->numberOfTests++;

	free(newTest);
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
	if(testSuit->tests != NULL)
	{
		TestPtr test = NULL;

		int t = 0;
		for (; t < testSuit->numberOfTests; t++)
		{
			test = &testSuit->tests[t];
			free(test->testCase);
			free(test->testName);
		}

		free(testSuit->tests);
	}

	if(testSuit->failTests != NULL)
	{
		free(testSuit->failTests);
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

	int t = 0;
    int count = testSuit->numberOfTests;
	int prevNumOfSuccessTestFuncs = 0, prevNumOfFailTestFuncs = 0;

    printf("\n[ 총 테스트 수: %d 개 ]\n", count);

    if (count >= 1)
    {
        // Call all test functions in the array
        TestPtr test = NULL;
        for (t = 0; t < count; t++)
        {
            test = &testSuit->tests[t];
            if (test->testFunc == NULL) // testSuit->tests is a NULL-terninated array
            {
                break;
            }

            printf("\n{ (테스트 번호: %d) 테스트 케이스: %s, 테스트 이름: %s }\n", (t + 1), test->testCase, test->testName);
            test->testFunc(testSuit);

			test->numberOfSuccessTestFunc = testSuit->totalNumOfSuccessTestFuncs - prevNumOfSuccessTestFuncs;
			test->numberOfFailTestFunc = testSuit->totalNumOfFailTestFuncs - prevNumOfFailTestFuncs;
			prevNumOfSuccessTestFuncs = testSuit->totalNumOfSuccessTestFuncs;
			prevNumOfFailTestFuncs = testSuit->totalNumOfFailTestFuncs;

			if(test->numberOfFailTestFunc == 0)
			{
				testSuit->numberOfSuccessTests++;
				printf("< 모든 테스트 케이스 성공 >\n");
			}
			else{
				testSuit->failTests[t] = test;
				testSuit->numberOfFailTests++;
			}
	    	printf("{ 성공: %d 개 / 실패: %d 개 }\n", test->numberOfSuccessTestFunc, test->numberOfFailTestFunc);
        }

        puts("\ncalling tests done.\n");
	    printf("[ 총 성공 테스트 함수 개수: %d 개 / 실패 테스트 함수 개수: %d 개 ]\n", testSuit->totalNumOfSuccessTestFuncs, testSuit->totalNumOfFailTestFuncs);
	    printf("[ 총 성공 테스트 수: %d 개 / 실패 테스트 수: %d 개 ]\n\n", testSuit->numberOfSuccessTests, testSuit->numberOfFailTests);

		if(testSuit->failTests != NULL)
		{
			printf("[ 실패한 테스트 목록 ]\n");
			printf("{ (테스트 케이스) : (테스트 이름) }\n");
			int failTestsIndex = 0;
			for( ; failTestsIndex < count; failTestsIndex++)
			{
			    printf("{ %s : %s }\n", (testSuit->failTests[failTestsIndex])->testCase, (testSuit->failTests[failTestsIndex])->testName);
			}
			printf("\n");
		}
    }
	else
	{
        puts("\n[ 테스트가 존재하지 않음. ]\n\n");
	}
}

void ProcessSuccessTestSuit(TestSuitPtr testSuit, const char *msg)
{
    // Check parameter
    if (testSuit == NULL)
    {
        return;
    }

	testSuit->totalNumOfSuccessTestFuncs++;

	if(msg != NULL)
	{
		TEST_SUCCESS(msg);
	}
}

void ProcessFailTestSuit(TestSuitPtr testSuit, const char *msg, const char *fileName, int lineNumber)
{
	// Check parameter
    if (testSuit == NULL)
    {
        return;
    }

	testSuit->totalNumOfFailTestFuncs++;

	if(msg != NULL)
	{
		TEST_NONFATAL_FAIL(msg, fileName, lineNumber);
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

	// initialize failTests in testSuit
	if(testSuit->numberOfTests > 0)
	{
		testSuit->failTests = (TestPtrContainer)malloc(sizeof(TestPtr) * (size_t)(testSuit->numberOfTests));
		if(testSuit->failTests == NULL)
		{
			return;
		}
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

void printMessageHelper(const char *file_name, int line_number, const char *msg, TEST_RESULT testResultType)
{
	switch(testResultType)
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

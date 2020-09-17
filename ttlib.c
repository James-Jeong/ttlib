#include "ttlib.h"

////////////////////////////////////////////////////////////////////////////////
/// Predefitions of Static Functions
//////////////////////////////////////////////////////////////////////////////////

static void initializeTests(TestSuitPtr testSuit);
static TestPtrContainer newTests(size_t numberOfTests);
static void deleteTest(TestPtr test);
static void printTests(TestPtrContainer tests, int numberOfTests);

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
	testSuit->successTests = NULL;
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

	if(testSuit->successTests != NULL)
	{
		free(testSuit->successTests);
	}

    // release memory allocated to the TestSuit instance
    free(testSuit);

    // set NULL to the container of pointer to TestSuit instance
    *testSuitContainer = NULL;
}

void RunAllTests(TestSuitPtr testSuit)
{
    // Check parameter
	if(testSuit == NULL)
	{
		return;
	}

    // Add user tests into TestSuit instance
    initializeTests(testSuit);

	int t = 0;
    int numberOfTests = testSuit->numberOfTests;
	int prevNumOfSuccessTestFuncs = 0, prevNumOfFailTestFuncs = 0;

   	printf("--------------------------------\n");
    printf("[ 총 테스트 수: %d 개 ]\n", numberOfTests);
   	printf("--------------------------------\n");

    if (numberOfTests >= 1)
    {
        // Call all test functions in the array
        TestPtr test = NULL;
        for (t = 0; t < numberOfTests; t++)
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
				testSuit->successTests[(testSuit->numberOfSuccessTests++)] = test;
				printf("< 모든 테스트 함수 성공 >\n");
			}
			else{
				testSuit->failTests[(testSuit->numberOfFailTests++)] = test;
			}
	    	printf("{ 성공: %d 개 / 실패: %d 개 }\n", test->numberOfSuccessTestFunc, test->numberOfFailTestFunc);
        }

    	printf("\n--------------------------------\n");
	    printf("[ 총 성공 테스트 함수 개수: %d 개 / 실패 테스트 함수 개수: %d 개 ]\n", testSuit->totalNumOfSuccessTestFuncs, testSuit->totalNumOfFailTestFuncs);
	    printf("[ 총 성공 테스트 수: %d 개 / 실패 테스트 수: %d 개 ]\n\n", testSuit->numberOfSuccessTests, testSuit->numberOfFailTests);

		printf("[ 성공한 테스트 목록 ]\n");
		printTests(testSuit->successTests, testSuit->numberOfSuccessTests);
		printf("\n");

		printf("[ 실패한 테스트 목록 ]\n");
		printTests(testSuit->failTests, testSuit->numberOfFailTests);

   		printf("--------------------------------\n");
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

void ProcessFailTestSuit(TestSuitPtr testSuit, const char *msg, const char *functionName, const char *fileName, int lineNumber)
{
	// Check parameter
    if (testSuit == NULL)
    {
        return;
    }

	testSuit->totalNumOfFailTestFuncs++;

	if(msg != NULL)
	{
		TEST_NONFATAL_FAIL(msg, functionName, fileName, lineNumber);
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
		size_t numberOfTests = (size_t)testSuit->numberOfTests;

		testSuit->failTests = newTests(numberOfTests);
		if(testSuit->failTests == NULL)
		{
			return;
		}

		testSuit->successTests = newTests(numberOfTests);
		if(testSuit->successTests == NULL)
		{
			return;
		}
	}
}

static TestPtrContainer newTests(size_t numberOfTests)
{
	TestPtrContainer tests = (TestPtrContainer)malloc(sizeof(TestPtr) * numberOfTests);
	if(tests == NULL)
	{
		return NULL;
	}

	size_t testIndex = 0;
	for( ; testIndex < numberOfTests; testIndex++)
	{
		tests[testIndex] = NULL;
	}

	return tests;
}

static void deleteTest(TestPtr test)
{
    // Check parameter
    if (test == NULL)
    {
        return;
    }

    free(test->testCase);
    free(test->testName);
    free(test);
}

static void printTests(TestPtrContainer tests, int numberOfTests)
{
    // Check parameter
	if(tests == NULL)
	{
		return;
	}

	printf("{ (테스트 케이스) : (테스트 이름) }\n");
	int testIndex = 0;
	TestPtr test = NULL;
	for( ; testIndex < numberOfTests; testIndex++)
	{
		test = tests[testIndex];
		if(test != NULL) printf("{ %s : %s }\n", test->testCase, test->testName);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// Util Functions
//////////////////////////////////////////////////////////////////////////////////

void printMessageHelper(const char *functionName, const char *file_name, int line_number, const char *msg, TEST_RESULT testResultType)
{
	switch(testResultType)
	{
		case SUCCESS:
			printf("[SUCCESS]\n");
			break;
		case FATAL_FAIL:
			printf("[%s FAIL] %s (file:%s, line:%d)\nTest aborted.\n", functionName, msg, file_name, line_number);
			break;
		case NON_FATAL_FAIL:
			printf("[%s FAIL] %s (file:%s, line:%d)\n", functionName, msg, file_name, line_number);
			break;
		default:
			printf("[%s UNKNOWN] %s (file:%s, line:%d)\n", functionName, msg, file_name, line_number);
			break;
	}	
}

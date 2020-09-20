#include "../include/ttlib.h"

////////////////////////////////////////////////////////////////////////////////
/// Predefitions of Static Functions
//////////////////////////////////////////////////////////////////////////////////

static TestInitializationResult initializeTests(TestSuitPtr testSuit);
static TestPtrContainer newTests(size_t numberOfTests);
static void deleteTest(TestPtr test);
static void printTests(const TestPtrContainer testPtrContainer, int numberOfTests);

////////////////////////////////////////////////////////////////////////////////
/// Local Functions
//////////////////////////////////////////////////////////////////////////////////

/**
 * @fn TestSuitPtr NewTestSuit()
 * @brief 전체 테스트 관리를 위한 TestSuit 객체를 새로 생성하는 함수
 * @return 성공 시 새로 생성된 TestSuit 객체, 실패 시 NULL 반환
 */
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
	testSuit->numberOfFailTests = 0;
	testSuit->totalNumOfFailTestFuncs = 0;
	testSuit->testPtrContainer = NULL;
	testSuit->onGoing = 0;

	return testSuit;
}

/**
 * @fn TestPtr AddTest(TestSuitPtr testSuit, Test test)
 * @brief 테스트를 추가하는 함수
 * @param testSuit 전체 테스트 관리 구조체(출력)
 * @param test 추가할 테스트(입력)
 * @return 성공 시 새로 추가된 테스트 구조체 주소, 실패 시 NULL 반환
 */
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

	// Reallocate memory for testPtrContainer in TestSuit instance
	int numberOfTests = testSuit->numberOfTests;
	size_t newSize = 0;
	TestPtrContainer newContainer = NULL;

	if (testSuit->testPtrContainer == NULL) // None of testPtrContainer registered yet.
	{
		newContainer = (TestPtrContainer)malloc(sizeof(TestPtr) * (size_t)(numberOfTests + 1));
	}
	else // One or more testPtrContainer exist already.
	{
		newSize = sizeof(TestPtr) * (size_t)(numberOfTests + 1);
		newContainer = (TestPtrContainer)realloc(testSuit->testPtrContainer, newSize);
	}
	if (newContainer == NULL)
	{
		deleteTest(newTest);
		return NULL;
	}

	// Add new Test instance pointer at the end of the TestSuit instance
	newContainer[testSuit->numberOfTests] = newTest;

	// Apply results to any related members
	testSuit->testPtrContainer = newContainer;
	testSuit->numberOfTests++;

	return newTest;
}

/**
 * @fn void DeleteTestSuit(TestSuitPtrContainer testSuitContainer)
 * @brief 전체 테스트 관리 구조체를 삭제하는 함수
 * @param testSuitContainer 전체 테스트 관리 구조체의 주소를 가지는 포인터(입력, 이중 포인터)
 * @return 반환값 없음
 */
void DeleteTestSuit(TestSuitPtrContainer testSuitContainer)
{
	// Check parameter
	if (testSuitContainer == NULL || *testSuitContainer == NULL)
	{
		return;
	}

	TestSuitPtr testSuit = *testSuitContainer;

	// release memory allocated to the array of Test instances
	if (testSuit->testPtrContainer != NULL)
	{
		TestPtr test = NULL;

		int testIndex = 0;
		for (; testIndex < testSuit->numberOfTests; testIndex++)
		{
			test = testSuit->testPtrContainer[testIndex];
			if(test != NULL)
			{
				free(test->testCase);
				free(test->testName);
				free(test);
			}
		}

		free(testSuit->testPtrContainer);
	}

	// release memory allocated to the TestSuit instance
	free(testSuit);

	// set NULL to the container of pointer to TestSuit instance
	*testSuitContainer = NULL;
}

/**
 * @fn void RunAllTests(TestSuitPtr testSuit)
 * @brief 전체 테스트들을 실행하는 함수
 * @param testSuit 전체 테스트 관리 구조체(입력 및 출력)
 * @return 반환값 없음
 */
void RunAllTests(TestSuitPtr testSuit)
{
	// Check parameter
	if (testSuit == NULL)
	{
		return;
	}

	// Add user testPtrContainer into TestSuit instance
	if (initializeTests(testSuit) == TestInitializationResultFail)
	{
		return;
	}

	int testIndex = 0;
	int numberOfTests = testSuit->numberOfTests;

	printf("--------------------------------\n");
	printf("[ 총 테스트 수: %d 개 ]\n", numberOfTests);
	printf("--------------------------------\n");

	if (numberOfTests >= 1)
	{
		// Call all test functions in the array
		TestPtr test = NULL;
		for (testIndex = 0; testIndex < numberOfTests; testIndex++)
		{
			test = testSuit->testPtrContainer[testIndex];
			if (test->testFunc == NULL) // testSuit->testPtrContainer is a NULL-terninated array
			{
				break;
			}

			printf("\n{ (테스트 번호: %d) 테스트 케이스: %s, 테스트 이름: %s }\n", (testIndex + 1), test->testCase, test->testName);

			test->testFunc();
			if (testSuit->onGoing == TestExit)
			{
				break;
			}
		}

		printf("\n--------------------------------\n");
		printf("[ 총 성공 테스트 수: %d 개 / 실패 테스트 수: %d 개 ]\n", numberOfTests - testSuit->totalNumOfFailTestFuncs, 
				testSuit->totalNumOfFailTestFuncs);
		printf("--------------------------------\n");
	}
	else
	{
		puts("\n[ 테스트가 존재하지 않음. ]\n");
	}
}

/**
 * @fn TestResult ProcessFailTestSuit(TestSuitPtr testSuit, const char *msg, const char *functionName, const char *fileName, int lineNumber, int failCount)
 * @brief 테스트 실패 시 테스트 실패 횟수를 하나 증가하고 실패 메시지를 출력하는 함수
 * @param testSuit 전체 테스트 관리 구조체(출력)
 * @param msg 실패 메시지(입력, 읽기 전용)
 * @param functionName 실패한 함수 이름(입력, 읽기 전용)
 * @param fileName 실패한 테스트가 작성된 파일 이름(입력, 읽기 전용)
 * @param lineNumber 실패한 테스트가 작성된 파일에서 호출된 코드의 라인 번호(입력)
 * @return 현재 진행 중인 테스트 종료하면 TestExit, 계속 진행하면 TestContinue, 실패하면 TestFail 반환
 */
TestResult ProcessFailTestSuit(TestSuitPtr testSuit, const char *msg, const char *functionName, const char *fileName, int lineNumber, int failCount)
{
	// Check parameter
	if (testSuit == NULL)
	{
		return TestFail;
	}

	testSuit->totalNumOfFailTestFuncs += failCount;

	if (functionName != NULL && functionName[0] == 'A')
	{
		PRINT_FATAL(msg, functionName, fileName, lineNumber);
		return TestExit;
	}

	if (functionName != NULL && fileName != NULL && msg != NULL && lineNumber > 0)
	{
		PRINT_NONFATAL(msg, functionName, fileName, lineNumber);
	}

	return TestContinue;
}

/**
 * @fn void SetExitTestSuit(TestSuitPtr testSuit)
 * @brief ASSERT 함수 호출인 경우 테스트 함수 동작 실패 시 테스트를 종료하도록 설정하는 함수
 * @param testSuit 전체 테스트 관리 구조체(출력)
 * @return 반환값 없음
 */
void SetExitTestSuit(TestSuitPtr testSuit)
{
	// Check parameter
	if (testSuit == NULL)
	{
		return;
	}

	testSuit->onGoing = TestExit;
}

/**
 * @fn void SetContinueTestSuit(TestSuitPtr testSuit)
 * @brief ASSERT 함수 호출인 경우 테스트 함수 동작 성공 시 테스트를 계속 진행하도록 설정하는 함수
 * @param testSuit 전체 테스트 관리 구조체(출력)
 * @return 반환값 없음
 */
void SetContinueTestSuit(TestSuitPtr testSuit)
{
	// Check parameter
	if (testSuit == NULL)
	{
		return;
	}

	testSuit->onGoing = TestContinue;
}

////////////////////////////////////////////////////////////////////////////////
/// Static Functions
//////////////////////////////////////////////////////////////////////////////////

/**
 * @fn static TestInitializationResult initializeTests(TestSuitPtr testSuit)
 * @brief 사용자가 작성한 테스트 함수들을 전체 테스트 관리 구조체(TestSuit)에 등록하는 함수
 * RunAllTests 함수에서 호출되기 때문에 전달받은 구조체 포인터에 대한 NULL 체크를 수행하지 않는다.
 * @param testSuit 전체 테스트 관리 구조체(입력 및 출력)
 * @return 성공 시 TestInitializationResultSuccess, 실패 시 TestInitializationResultSuccessFail 반환
 */
static TestInitializationResult initializeTests(TestSuitPtr testSuit)
{
	TestSuitInitializer *initializers = testSuit->initializers;
	if (initializers == NULL)
	{
		return TestInitializationResultFail;
	}

	TestSuitInitializer initializer = NULL;
	while ((initializer = *initializers++) != NULL)
	{
		initializer();
	}

	return TestInitializationResultSuccess;
}

/**
 * @fn static TestPtrContainer newTests(size_t numberOfTests)
 * @brief TestPtrContainer 를 새로 생성하는 함수
 * initializeTests 함수에서 호출되기 때문에 전달받은 크기 체크를 수행하지 않는다.
 * @param numberOfTests 저장할 Test 구조체 포인터의 개수(입력)
 * @return 성공 시 새로 생성된 TestPtrContainer 객체, 실패 시 NULL 반환
 */
static TestPtrContainer newTests(size_t numberOfTests)
{
	TestPtrContainer testPtrContainer = (TestPtrContainer)malloc(sizeof(TestPtr) * numberOfTests);
	if (testPtrContainer == NULL)
	{
		return NULL;
	}

	size_t testIndex = 0;
	for (; testIndex < numberOfTests; testIndex++)
	{
		testPtrContainer[testIndex] = NULL;
	}

	return testPtrContainer;
}

/**
 * @fn static void deleteTest(TestPtr test)
 * @brief Test 구조체를 삭제하는 함수
 * AddTest 함수에서 호출되기 때문에 전달받은 구조체 포인터에 대한 NULL 체크를 수행하지 않는다.
 * @param test 삭제할 Test 구조체(입력)
 * @return 반환값 없음
 */
static void deleteTest(TestPtr test)
{
	free(test->testCase);
	free(test->testName);
	free(test);
}

////////////////////////////////////////////////////////////////////////////////
/// Util Functions
//////////////////////////////////////////////////////////////////////////////////

/**
 * @fn void printMessageHelper(const char *functionName, const char *file_name, int line_number, const char *msg, TestResult testResultType)
 * @brief 함수 결과에 따라 출력 방법을 다르게 설정하여 지정한 메시지를 출력하는 함수
 * ProcessFailTestSuit 함수에서 호출되기 때문에 전달받은 문자열에 대한 NULL 체크를 수행하지 않는다.
 * @param functionName 실패한 함수 이름(입력, 읽기 전용)
 * @param fileName 실패한 테스트가 작성된 파일 이름(입력, 읽기 전용)
 * @param lineNumber 실패한 테스트가 작성된 파일에서 호출된 코드의 라인 번호(입력)
 * @param msg 출력할 메시지(입력, 읽기 전용)
 * @param testResultType 테스트 함수 실행 결과(입력)
 * @return 반환값 없음
 */
void printMessageHelper(const char *functionName, const char *file_name, int line_number, const char *msg, TestResult testResultType)
{
	switch (testResultType)
	{
		case TestSuccess:
			//printf("[SUCCESS]\n");
			break;
		case TestFatal:
			printf("[%s FAIL] %s (file:%s, line:%d)\nTest aborted.\n", functionName, msg, file_name, line_number);
			break;
		case TestNonFatal:
			printf("[%s FAIL] %s (file:%s, line:%d)\n", functionName, msg, file_name, line_number);
			break;
		default:
			printf("[%s UNKNOWN] %s (file:%s, line:%d)\n", functionName, msg, file_name, line_number);
			break;
	}
}

/**
 * @fn int CheckIsUpperString(const char *string)
 * @brief 지정한 문자열 전체가 대문자로 구성되어 있는지 검사하는 함수
 * @param actual 검사할 문자열(입력, 읽기 전용)
 * @return 성공 시 소문자 개수, 실패 시 -1 반환
 */
int CheckIsUpperString(const char *string)
{
	if(string == NULL)
	{
		return -1;
	}

	int failCount = 0;
	int charIndex = 0;
	int length = strlen(string);
	for( ; charIndex < length; charIndex++)
	{
		if(isupper(string[charIndex]) == 0) failCount++;
	}

	return failCount;
}

/**
 * @fn int CheckIsLowerString(const char *string)
 * @brief 지정한 문자열 전체가 소문자로 구성되어 있는지 검사하는 함수
 * @param actual 검사할 문자열(입력, 읽기 전용)
 * @return 성공 시 대문자 개수, 실패 시 -1 반환
 */
int CheckIsLowerString(const char *string)
{
	if(string == NULL)
	{
		return -1;
	}

	int failCount = 0;
	int charIndex = 0;
	int length = strlen(string);
	for( ; charIndex < length; charIndex++)
	{
		if(islower(string[charIndex]) == 0) failCount++;
	}

	return failCount;
}


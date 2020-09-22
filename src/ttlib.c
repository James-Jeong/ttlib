#include "../include/ttlib.h"

////////////////////////////////////////////////////////////////////////////////
/// Predefitions of Static Functions
////////////////////////////////////////////////////////////////////////////////

static TestInitializationResult InitializeTests(const TestSuitPtr testSuit);
static TestPtrContainer NewTestPtrContainer(size_t numberOfTests);
static void DeleteTest(TestPtr test);
static void DeleteTestPtrContainer(TestPtrContainer testPtrContainer, int numberOfTests);

////////////////////////////////////////////////////////////////////////////////
/// Local Functions
////////////////////////////////////////////////////////////////////////////////

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
	testSuit->testPtrContainer = NULL;
	testSuit->numberOfTests = 0;
	testSuit->numberOfFailTests = 0;
	testSuit->onGoing = TestContinue;

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
		newContainer = NewTestPtrContainer((size_t)(numberOfTests + 1));
	}
	else // One or more testPtrContainer exist already.
	{
		newSize = sizeof(TestPtr) * (size_t)(numberOfTests + 1);
		newContainer = (TestPtrContainer)realloc(testSuit->testPtrContainer, newSize);
	}
	if (newContainer == NULL)
	{
		DeleteTest(newTest);
		return NULL;
	}

	// Add new Test instance pointer at the end of the TestPtrContainer instance
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
	if ((testSuit->testPtrContainer != NULL) && (testSuit->numberOfTests > 0))
	{
		DeleteTestPtrContainer(testSuit->testPtrContainer, testSuit->numberOfTests);
	}

	// release memory allocated to the TestSuit instance
	free(testSuit);

	// set NULL to the container of pointer to TestSuit instance
	*testSuitContainer = NULL;
}

/**
 * @fn void RunAllTests(TestSuitPtr testSuit)
 * @brief 전체 테스트들을 실행하는 함수
 * @param testSuit 전체 테스트 관리 구조체(입력)
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
	if (InitializeTests(testSuit) == TestInitializationResultFail)
	{
		return;
	}

	int numberOfCurTests = 0;
	int numberOfTests = testSuit->numberOfTests;

	printf("--------------------------------\n");
	printf("[ 총 테스트 수: %d 개 ]\n", numberOfTests);
	printf("--------------------------------\n");

	if (numberOfTests >= 1)
	{
		// Call all test functions in the array
		TestPtr test = NULL;
		while(numberOfCurTests < numberOfTests)
		{
			test = testSuit->testPtrContainer[numberOfCurTests];
			if (test->testFunc == NULL) // testSuit->testPtrContainer is a NULL-terninated array
			{
				break;
			}

			printf("\n{ (테스트 번호: %d) 테스트 케이스: %s, 테스트 이름: %s }\n", (numberOfCurTests + 1), test->testCase, test->testName);

			test->testFunc(testSuit);
			numberOfCurTests++;
			if (testSuit->onGoing == TestExit) break;
		}

		printf("\n--------------------------------\n");
		printf("[ 총 성공 테스트 수: %d 개 / 실패 테스트 수: %d 개 ]\n", numberOfCurTests - testSuit->numberOfFailTests, testSuit->numberOfFailTests);
		printf("--------------------------------\n");
	}
	else
	{
		puts("\n[ 테스트가 존재하지 않음. ]\n");
	}
}

/**
 * @fn void IncFailCountTestSuit(TestSuitPtr testSuit)
 * @brief 테스트 함수 실패 시 증가한 failCount 를 testSuit 구조체 저장하는 함수
 * @param testSuit 전체 테스트 관리 구조체(출력)
 * @param failCount 테스트 함수 실패 횟수(입력)
 * @return 반환값 없음
 */
void IncFailCountTestSuit(TestSuitPtr testSuit)
{
	// Check parameter
	if (testSuit == NULL)
	{
		return;
	}

	testSuit->numberOfFailTests++;
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

////////////////////////////////////////////////////////////////////////////////
/// Static Functions
////////////////////////////////////////////////////////////////////////////////

/**
 * @fn static TestInitializationResult InitializeTests(TestSuitPtr testSuit)
 * @brief 사용자가 작성한 테스트 함수들을 전체 테스트 관리 구조체(TestSuit)에 등록하는 함수
 * RunAllTests 함수에서 호출되기 때문에 전달받은 구조체 포인터에 대한 NULL 체크를 수행하지 않는다.
 * @param testSuit 전체 테스트 관리 구조체(입력, 읽기 전용)
 * @return 성공 시 TestInitializationResultSuccess, 실패 시 TestInitializationResultSuccessFail 반환
 */
static TestInitializationResult InitializeTests(const TestSuitPtr testSuit)
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
 * @fn static TestPtrContainer NewTestPtrContainer(size_t numberOfTests)
 * @brief TestPtrContainer 를 새로 생성하는 함수
 * @param numberOfTests 저장할 Test 구조체 포인터의 개수(입력)
 * @return 성공 시 새로 생성된 TestPtrContainer 객체, 실패 시 NULL 반환
 */
static TestPtrContainer NewTestPtrContainer(size_t numberOfTests)
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
 * @fn static void DeleteTestPtrContainer(TestPtrContainer testPtrContainer, int numberOfTests)
 * @brief TestPtrContainer 를 삭제하는 함수
 * DeleteTestSuit 함수에서 호출되기 때문에 전달받은 구조체의 이중 포인터에 대한 NULL 체크와 numberOfTests 에 대한 체크를 수행하지 않는다.
 * @param testPtrContainer 삭제할 구조체의 포인터들을 가지는 이중 포인터(입력, 이중 포인터)
 * @param numberOfTests 저장된 Test 구조체 포인터의 개수(입력)
 * @return 반환값 없음
 */
static void DeleteTestPtrContainer(TestPtrContainer testPtrContainer, int numberOfTests)
{
	TestPtr test = NULL;

	int testIndex = 0;
	for (; testIndex < numberOfTests; testIndex++)
	{
		test = testPtrContainer[testIndex];
		if(test != NULL)
		{
			free(test->testCase);
			free(test->testName);
			free(test);
		}
	}

	free(testPtrContainer);
}

/**
 * @fn static void DeleteTest(TestPtr test)
 * @brief Test 구조체를 삭제하는 함수
 * AddTest 함수에서 호출되기 때문에 전달받은 구조체 포인터에 대한 NULL 체크를 수행하지 않는다.
 * @param test 삭제할 Test 구조체(입력)
 * @return 반환값 없음
 */
static void DeleteTest(TestPtr test)
{
	free(test->testCase);
	free(test->testName);
	free(test);
}


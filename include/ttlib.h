#ifndef __TTLIB_H__
#define __TTLIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//////////////////////////////////////////////////////////////////////////////////
/// Enums
//////////////////////////////////////////////////////////////////////////////////

/**
 * @enum TestResult
 * @brief 테스트 함수 실행 결과에 대한 상태값을 지정하기 위한 열거형
 */
typedef enum _TestResult_t
{
	// 테스트 실패(매크로 함수용)
	TestFail = -1,
	// 테스트 성공
	TestSuccess = 1,
	// 테스트 실패(fatal, 테스트 종료)
	TestFatal,
	// 테스트 실패(non-fatal, 테스트 계속 진행)
	TestNonFatal,
	// 테스트 계속 실행
	TestContinue,
	// 테스트 종료
	TestExit
} TestResult;

/**
 * @enum TestInitializationResult
 * @brief 함수 실행 결과에 대한 상태값을 지정하기 위한 열거형
 */

typedef enum _testInitializationResult_t
{
	// 초기화 실패
	TestInitializationResultFail = -1,
	// 초기화 성공
	TestInitializationResultSuccess = 1,
} TestInitializationResult;

//////////////////////////////////////////////////////////////////////////////////
/// Macros
//////////////////////////////////////////////////////////////////////////////////

// 조건 참
#ifndef TRUE
#define TRUE 1
#endif

// 조건 거짓
#ifndef FALSE
#define FALSE 0
#endif

//////////////////////////////////////////////////////////////////////////////////
/// Control Macro Functions
//////////////////////////////////////////////////////////////////////////////////

// 테스트 진행을 위한 TestSuit 객체를 전역변수로 선언하기 위한 함수
#define DECLARE_TEST() static TestSuitPtr _testSuit = NULL;

// 테스트 함수를 설정하고 TestSuit 객체에 추가하기 위한 함수
#define TEST(C, T, F) \
	TestResult _##C##_##T() \
{ \
	int failCount = 0; \
	F; \
	if(failCount > 0){ \
		return TestFail; \
	} \
	return TestSuccess; \
} \
void Test_##C##_##T() \
{ \
	if(AddTest(_testSuit, (Test){#C, #T, _##C##_##T}) == NULL) { \
		puts("테스트 추가 실패"); \
		return; \
	} \
}

// TestSuit 객체를 새로 생성하는 함수
#define CREATE_TESTSUIT() \
	_testSuit = NewTestSuit(); \
if (_testSuit == NULL) { \
	printf("failed to create a test suit instance!\n"); \
	exit(-1); \
}

// 테스트 함수를 등록하기 위한 함수
#define REGISTER_TESTS(X...) \
	_testSuit->initializers = (TestSuitInitializer[]){ \
		X, \
		NULL};

// 모든 테스트 함수를 동작시키기 위한 함수
#define RUN_ALL_TESTS() RunAllTests(_testSuit)

// TestSuit 객체를 삭제하는 함수
#define CLEAN_UP_TESTSUIT() DeleteTestSuit(&_testSuit)

//////////////////////////////////////////////////////////////////////////////////
/// Print Macro Functions
//////////////////////////////////////////////////////////////////////////////////

// 테스트 결과를 성공/실패 여부에 따라 출력하는 함수
#define PRINT_MESSAGE(msg, testResultType, functionName, fileName, lineNumber) printMessageHelper(functionName, fileName, lineNumber, msg, testResultType)

// 테스트 성공 시 출력할 내용을 지정하는 함수
#define PRINT_SUCCESS(msg) PRINT_MESSAGE(msg, TestSuccess, NULL, NULL, 0)

// 테스트 실패 시(fatal) 출력할 내용을 지정하는 함수
#define PRINT_FATAL(msg, functionName, fileName, lineNumber) PRINT_MESSAGE(msg, TestFatal, functionName, fileName, lineNumber)

// 테스트 실패 시(nonfatal) 출력할 내용을 지정하는 함수
#define PRINT_NONFATAL(msg, functionName, fileName, lineNumber) PRINT_MESSAGE(msg, TestNonFatal, functionName, fileName, lineNumber)

//////////////////////////////////////////////////////////////////////////////////
/// (EXPECT) NULL Macro Functions
/// 실패해도 현재 진행 중인 테스트가 종료되지 않는다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 값이 NULL 인지 검사하는 함수
#define EXPECT_NULL(actual) ((actual) == NULL) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 NULL 이어야 한다.", "EXPECT_NULL", __FILE__, __LINE__, ++failCount))

// 실제 값이 NULL 이 아닌지 검사하는 함수
#define EXPECT_NOT_NULL(actual) ((actual) != NULL) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 NULL 이 아니어야 한다.", "EXPECT_NOT_NULL", __FILE__, __LINE__, ++failCount))

//////////////////////////////////////////////////////////////////////////////////
/// (EXPECT) Number Macro Functions
/// 실패해도 현재 진행 중인 테스트가 종료되지 않는다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 값과 기대하는 값이 같은지 검사하는 함수
#define EXPECT_NUM_EQUAL(actual, expected) ((actual) == (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값과 "#expected" 값이 달라야 한다.", "EXPECT_NUM_EQUAL", __FILE__, __LINE__, ++failCount))

// 실제 값과 기대하는 값이 다른지 검사하는 함수
#define EXPECT_NUM_NOT_EQUAL(actual, expected) ((actual) != (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값과 "#expected" 값이 같아야 한다.", "EXPECT_NUM_NOT_EQUAL", __FILE__, __LINE__, ++failCount))

// 실제 값이 기대하는 값보다 작거나 같은지 검사하는 함수
#define EXPECT_NUM_LESS_EQUAL(actual, expected) ((actual) <= (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 작아야 한다.", "EXPECT_NUM_LESS_EQUAL", __FILE__, __LINE__, ++failCount))

// 실제 값이 기대하는 값보다 작은지 검사하는 함수
#define EXPECT_NUM_LESS_THAN(actual, expected) ((actual) < (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 작거나 같아야 한다.", "EXPECT_NUM_LESS_THAN", __FILE__, __LINE__, ++failCount))

// 실제 값이 기대하는 값보다 크거나 같은지 검사하는 함수
#define EXPECT_NUM_GREATER_EQUAL(actual, expected) ((actual) >= (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 커야 한다.", "EXPECT_NUM_GREATER_EQUAL", __FILE__, __LINE__, ++failCount))

// 실제 값이 기대하는 값보다 큰지 검사하는 함수
#define EXPECT_NUM_GREATER_THAN(actual, expected) ((actual) > (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 크거나 같아야 한다.", "EXPECT_NUM_GREATER_THAN", __FILE__, __LINE__, ++failCount))

// 지정한 숫자가 짝수인지 검사하는 함수
#define EXPECT_NUM_EVEN(number) ((number) % 2 == 0) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #number" 값은 홀수여야 한다.", "EXPECT_NUM_EVEN", __FILE__, __LINE__, ++failCount))

// 지정한 숫자가 홀수인지 검사하는 함수
#define EXPECT_NUM_ODD(number) ((number) % 2 == 1) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #number" 값은 짝수여야 한다.", "EXPECT_NUM_ODD", __FILE__, __LINE__, ++failCount))

//////////////////////////////////////////////////////////////////////////////////
/// (EXPECT) String Macro Functions
/// 실패해도 현재 진행 중인 테스트가 종료되지 않는다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 문자열과 기대하는 문자열이 같은지 검사하는 함수
#define EXPECT_STR_EQUAL(actual, expected) (strncmp((actual), (expected), (strlen(expected) + 1)) == 0) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 문자열과 "#expected" 문자열이 같아야 한다.", "EXPECT_STR_EQUAL", __FILE__, __LINE__, ++failCount))

// 실제 문자열과 기대하는 문자열이 다른지 검사하는 함수
#define EXPECT_STR_NOT_EQUAL(actual, expected) (strncmp((actual), (expected), (strlen(expected) + 1)) != 0) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 문자열과 "#expected" 문자열이 달라야 한다.", "EXPECT_STR_NOT_EQUAL", __FILE__, __LINE__, ++failCount))

// 실제 문자열이 대문자인지 검사하는 함수
#define EXPECT_STR_UPPER_CASE(actual) (CheckIsUpperString(actual) == 0) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 문자가 대문자로 구성되어야 한다.", "EXPECT_STR_UPPER_CASE", __FILE__, __LINE__, ++failCount))

// 실제 문자열이 소문자인지 검사하는 함수
#define EXPECT_STR_LOWER_CASE(actual) (CheckIsLowerString(actual) == 0) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 문자열 소문자로 구성되어야 한다.", "EXPECT_STR_LOWER_CASE", __FILE__, __LINE__, ++failCount))

//////////////////////////////////////////////////////////////////////////////////
/// (ASSERT) NULL Macro Functions
/// 실패하면 현재 진행 중인 테스트가 종료된다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 값이 NULL 인지 검사하는 함수
#define ASSERT_NULL(actual) ((actual) == NULL) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 NULL 이어야 한다.", "ASSERT_NULL", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

// 실제 값이 NULL 이 아닌지 검사하는 함수
#define ASSERT_NOT_NULL(actual) ((actual) != NULL) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 NULL 이 아니어야 한다.", "ASSERT_NOT_NULL", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

//////////////////////////////////////////////////////////////////////////////////
/// (ASSERT) Number Macro Functions
/// 실패하면 현재 진행 중인 테스트가 종료된다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 값과 기대하는 값이 같은지 검사하는 함수 
#define ASSERT_NUM_EQUAL(actual, expected) ((actual) == (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값과 "#expected" 값이 같아야 한다.", "ASSERT_NUM_EQUAL", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

// 실제 값과 기대하는 값이 다른지 검사하는 함수
#define ASSERT_NUM_NOT_EQUAL(actual, expected) ((actual) != (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값과 "#expected" 값이 달라야 한다.", "ASSERT_NUM_NOT_EQUAL", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

// 실제 값이 기대하는 값보다 작거나 같은지 검사하는 함수
#define ASSERT_NUM_LESS_EQUAL(actual, expected) ((actual) <= (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 작아야 한다.", "ASSERT_NUM_LESS_EQUAL", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

// 실제 값이 기대하는 값보다 작은지 검사하는 함수
#define ASSERT_NUM_LESS_THAN(actual, expected) ((actual) < (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 작거나 같아야 한다.", "ASSERT_NUM_LESS_THAN", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

// 실제 값이 기대하는 값보다 크거나 같은지 검사하는 함수
#define ASSERT_NUM_GREATER_EQUAL(actual, expected) ((actual) >= (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 커야 힌다.", "ASSERT_NUM_GREATER_EQUAL", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

// 실제 값이 기대하는 값보다 큰지 검사하는 함수
#define ASSERT_NUM_GREATER_THAN(actual, expected) ((actual) > (expected)) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 크거나 같음.", "ASSERT_NUM_GREATER_THAN", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

// 지정한 숫자가 짝수인지 검사하는 함수
#define ASSERT_NUM_EVEN(number) ((number) % 2 == 0) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #number" 값은 홀수여야 한다.", "ASSERT_NUM_EVEN", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

// 지정한 숫자가 홀수인지 검사하는 함수
#define ASSERT_NUM_ODD(number) ((number) % 2 == 1) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #number" 값은 짝수여야 한다.", "ASSERT_NUM_ODD", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

//////////////////////////////////////////////////////////////////////////////////
/// (ASSERT) String Macro Functions
/// 실패하면 현재 진행 중인 테스트가 종료된다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 문자열과 기대하는 문자열이 같은지 검사하는 함수
#define ASSERT_STR_EQUAL(actual, expected) (strncmp((actual), (expected), (strlen(expected) + 1)) == 0) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 문자열과 "#expected" 문자열이 같아야 한다.", "ASSERT_STR_EQUAL", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

// 실제 문자열과 기대하는 문자열이 다른지 검사하는 함수
#define ASSERT_STR_NOT_EQUAL(actual, expected) (strncmp((actual), (expected), (strlen(expected) + 1)) != 0) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 문자열과 "#expected" 문자열이 달라야 한다.", "ASSERT_STR_NOT_EQUAL", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

// 실제 문자열이 대문자인지 검사하는 함수
#define ASSERT_STR_UPPER_CASE(actual) (CheckIsUpperString(actual) == 0) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 문자열이 대문자로 구성되어야 한다.", "ASSERT_STR_UPPER_CASE", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

// 실제 문자열이 소문자인지 검사하는 함수
#define ASSERT_STR_LOWER_CASE(actual) (CheckIsLowerString(actual) == 0) ? PRINT_SUCCESS(NULL) : (ProcessFailTestSuit(_testSuit, #actual" 문자열이 소문자로 구성되어야 한다.", "ASSERT_STR_LOWER_CASE", __FILE__, __LINE__, ++failCount) == TestExit ? SetExitTestSuit(_testSuit) : SetContinueTestSuit(_testSuit))

//////////////////////////////////////////////////////////////////////////////////
/// Definitions
//////////////////////////////////////////////////////////////////////////////////

// 테스트와 관련된 구조체들을 미리 정의
struct _test_t;
struct _test_suit_t;

// 실행될 테스트 함수의 주소를 저장할 함수 포인터
typedef TestResult (*TestFunc)();
// 테스트를 관리하는 TestSuit 객체에 저장된 테스트를 실행하기 위한 함수의 주소를 저장한 함수 포인터
typedef void (*TestSuitInitializer)();

// 사용자의 테스트 함수에 대한 정보를 관리하기 위한 구조체
typedef struct _test_t
{
	// 테스트 케이스 이름
	char *testCase;
	// 테스트 이름
	char *testName;
	// 실행될 테스트 함수
	TestFunc testFunc;
} Test, *TestPtr, **TestPtrContainer;

// 모든 사용자 테스트를 관리하기 위한 구조체
typedef struct _test_suit_t
{
	// 전체 테스트 개수
	int numberOfTests;
	// 테스트 전체 실패 횟수
	int numberOfFailTests;
	// 테스트 함수 전체 실패 횟수
	int totalNumOfFailTestFuncs;
	// 현재 진행 중인 테스트 실행 여부
	int onGoing;
	// 사용자가 작성한 테스트 등록 및 초기화를 담당하는 객체
	TestSuitInitializer *initializers;
	// 사용자가 작성한 테스트 리스트
	TestPtrContainer testPtrContainer;
} TestSuit, *TestSuitPtr, **TestSuitPtrContainer;

//////////////////////////////////////////////////////////////////////////////////
/// Local Functions
//////////////////////////////////////////////////////////////////////////////////

TestSuitPtr NewTestSuit();
void DeleteTestSuit(TestSuitPtrContainer testSuitContainer);

TestPtr AddTest(TestSuitPtr testSuit, Test test);
void RunAllTests(TestSuitPtr testSuit);

TestResult ProcessFailTestSuit(TestSuitPtr testSuit, const char *msg, const char *functionName, const char *fileName, int lineNumber, int failCount);

void SetExitTestSuit(TestSuitPtr testSuit);
void SetContinueTestSuit(TestSuitPtr testSuit);

//////////////////////////////////////////////////////////////////////////////////
/// Util Function
//////////////////////////////////////////////////////////////////////////////////

void printMessageHelper(const char *functionName, const char *file_name, int line_number, const char *msg, TestResult testResultType);
int CheckIsUpperString(const char *actual);
int CheckIsLowerString(const char *actual);

#endif

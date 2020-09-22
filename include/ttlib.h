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
	// 테스트 종료
	TestExit = -2,
	// 테스트 실패(매크로 함수 실행 결과에 사용)
	TestFail = -1,
	// 테스트 성공
	TestSuccess = 1,
	// 테스트 실패(fatal, 테스트 종료)
	TestFatal,
	// 테스트 실패(non-fatal, 테스트 계속 진행)
	TestNonFatal,
	// 테스트 계속 실행
	TestContinue
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
	TestResult _##C##_##T(TestSuitPtr testSuit) \
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
		exit(TestExit); \
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

// 테스트 성공 시 출력할 내용을 지정하는 함수
#define PRINT_SUCCESS(functionName, format, ...) ProcessSuccessTest(functionName, format, __VA_ARGS__)
// 테스트 실패 시 출력할 내용을 지정하는 함수
#define PRINT_FAIL(functionName, fileName, lineNumber, failCount, format, ...) \
	printf("(FAIL) ["functionName"] "format" (file:%s, line:%d)\n", __VA_ARGS__, fileName, lineNumber); \
	if(failCount > 0) IncFailCountTestSuit(testSuit); \
	if(functionName[0] == 'A') { \
		SetExitTestSuit(_testSuit); }

//////////////////////////////////////////////////////////////////////////////////
/// Format Macros
//////////////////////////////////////////////////////////////////////////////////
#define NULL_FORMAT "actual:%p, expected:NULL"
#define NUM1_FORMAT "actual:%d"
#define NUM2_FORMAT "actual:%d, expected:%d"
#define PTR_FORMAT "actual:%p, expected:%p"
#define STR_FORMAT "actual:%s, expected:%s"

//////////////////////////////////////////////////////////////////////////////////
/// (EXPECT) NULL Macro Functions
/// 실패해도 현재 진행 중인 테스트가 종료되지 않는다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 값이 NULL 인지 검사하는 함수
#define EXPECT_NULL(actual) ((actual) == NULL) ? PRINT_SUCCESS("EXPECT_NULL", NULL_FORMAT, actual) : PRINT_FAIL("EXPECT_NULL", __FILE__, __LINE__, ++failCount, NULL_FORMAT, actual)

// 실제 값이 NULL 이 아닌지 검사하는 함수
#define EXPECT_NOT_NULL(actual) ((actual) != NULL) ? PRINT_SUCCESS("EXPECT_NOT_NULL", NULL_FORMAT, actual) : PRINT_FAIL("EXPECT_NOT_NULL", __FILE__, __LINE__, ++failCount, NULL_FORMAT, actual)

//////////////////////////////////////////////////////////////////////////////////
/// (EXPECT) Pointer Macro Functions
/// 실패해도 현재 진행 중인 테스트가 종료되지 않는다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 주소값과 기대하는 주소값이 같은지 검사하는 함수
#define EXPECT_PTR_EQUAL(actual, expected) ((actual) == (expected)) ? PRINT_SUCCESS("EXPECT_PTR_EQUAL", PTR_FORMAT, actual, expected) : PRINT_FAIL("EXPECT_PTR_EQUAL", __FILE__, __LINE__, ++failCount, PTR_FORMAT, actual, expected)

// 실제 주소값과 기대하는 주소값이 다른지 검사하는 함수
#define EXPECT_PTR_NOT_EQUAL(actual, expected) ((actual) != (expected)) ? PRINT_SUCCESS("EXPECT_PTR_NOT_EQUAL", PTR_FORMAT, actual, expected) : PRINT_FAIL("EXPECT_PTR_NOT_EQUAL", __FILE__, __LINE__, ++failCount, PTR_FORMAT, actual, expected)

//////////////////////////////////////////////////////////////////////////////////
/// (EXPECT) Number Macro Functions
/// 실패해도 현재 진행 중인 테스트가 종료되지 않는다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 값과 기대하는 값이 같은지 검사하는 함수
#define EXPECT_NUM_EQUAL(actual, expected) ((actual) == (expected)) ? PRINT_SUCCESS("EXPECT_NUM_EQUAL", NUM2_FORMAT, actual, expected) : PRINT_FAIL("EXPECT_NUM_EQUAL", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 실제 값과 기대하는 값이 다른지 검사하는 함수
#define EXPECT_NUM_NOT_EQUAL(actual, expected) ((actual) != (expected)) ? PRINT_SUCCESS("EXPECT_NUM_NOT_EQUAL", NUM2_FORMAT, actual, expected) : PRINT_FAIL("EXPECT_NUM_NOT_EQUAL", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 실제 값이 기대하는 값보다 작거나 같은지 검사하는 함수
#define EXPECT_NUM_LESS_EQUAL(actual, expected) ((actual) <= (expected)) ? PRINT_SUCCESS("EXPECT_NUM_LESS_EQUAL", NUM2_FORMAT, actual, expected) : PRINT_FAIL("EXPECT_NUM_LESS_EQUAL", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 실제 값이 기대하는 값보다 작은지 검사하는 함수
#define EXPECT_NUM_LESS_THAN(actual, expected) ((actual) < (expected)) ? PRINT_SUCCESS("EXPECT_NUM_LESS_THAN", NUM2_FORMAT, actual, expected) : PRINT_FAIL("EXPECT_NUM_LESS_THAN", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 실제 값이 기대하는 값보다 크거나 같은지 검사하는 함수
#define EXPECT_NUM_GREATER_EQUAL(actual, expected) ((actual) >= (expected)) ? PRINT_SUCCESS("EXPECT_NUM_GREATER_EQUAL", NUM2_FORMAT, actual, expected) : PRINT_FAIL("EXPECT_NUM_GREATER_EQUAL", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 실제 값이 기대하는 값보다 큰지 검사하는 함수
#define EXPECT_NUM_GREATER_THAN(actual, expected) ((actual) > (expected)) ? PRINT_SUCCESS("EXPECT_NUM_GREATER_THAN", NUM2_FORMAT, actual, expected) : PRINT_FAIL("EXPECT_NUM_GREATER_THAN", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 지정한 숫자가 짝수인지 검사하는 함수
#define EXPECT_NUM_EVEN(actual) ((actual) % 2 == 0) ? PRINT_SUCCESS("EXPECT_NUM_EVEN", NUM1_FORMAT, actual) : PRINT_FAIL("EXPECT_NUM_EVEN", __FILE__, __LINE__, ++failCount, NUM1_FORMAT, actual)

// 지정한 숫자가 홀수인지 검사하는 함수
#define EXPECT_NUM_ODD(actual) ((actual) % 2 == 1) ? PRINT_SUCCESS("EXPECT_NUM_ODD", NUM1_FORMAT, actual) : PRINT_FAIL("EXPECT_NUM_ODD", __FILE__, __LINE__, ++failCount, NUM1_FORMAT, actual)

//////////////////////////////////////////////////////////////////////////////////
/// (EXPECT) String Macro Functions
/// 실패해도 현재 진행 중인 테스트가 종료되지 않는다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 문자열과 기대하는 문자열이 같은지 검사하는 함수
#define EXPECT_STR_EQUAL(actual, expected) (strncmp((actual), (expected), (strlen(expected) + 1)) == 0) ? PRINT_SUCCESS("EXPECT_STR_EQUAL", STR_FORMAT, actual, expected) : PRINT_FAIL("EXPECT_STR_EQUAL", __FILE__, __LINE__, ++failCount, STR_FORMAT, actual, expected)

// 실제 문자열과 기대하는 문자열이 다른지 검사하는 함수
#define EXPECT_STR_NOT_EQUAL(actual, expected) (strncmp((actual), (expected), (strlen(expected) + 1)) != 0) ? PRINT_SUCCESS("EXPECT_STR_NOT_EQUAL", STR_FORMAT, actual, expected) : PRINT_FAIL("EXPECT_STR_NOT_EQUAL", __FILE__, __LINE__, ++failCount, STR_FORMAT, actual, expected)

//////////////////////////////////////////////////////////////////////////////////
/// (ASSERT) NULL Macro Functions
/// 실패하면 현재 진행 중인 테스트가 종료된다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 값이 NULL 인지 검사하는 함수
#define ASSERT_NULL(actual) ((actual) == NULL) ? PRINT_SUCCESS("ASSERT_NULL", NULL_FORMAT, actual) : PRINT_FAIL("ASSERT_NULL", __FILE__, __LINE__, ++failCount, NULL_FORMAT, actual)

// 실제 값이 NULL 이 아닌지 검사하는 함수
#define ASSERT_NOT_NULL(actual) ((actual) != NULL) ? PRINT_SUCCESS("ASSERT_NOT_NULL", NULL_FORMAT, actual) : PRINT_FAIL("ASSERT_NOT_NULL", __FILE__, __LINE__, ++failCount, NULL_FORMAT, actual)

//////////////////////////////////////////////////////////////////////////////////
/// (ASSERT) Number Macro Functions
/// 실패하면 현재 진행 중인 테스트가 종료된다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 값과 기대하는 값이 같은지 검사하는 함수 
#define ASSERT_NUM_EQUAL(actual, expected) ((actual) == (expected)) ? PRINT_SUCCESS("ASSERT_NUM_EQUAL", NUM2_FORMAT, actual, expected) : PRINT_FAIL("ASSERT_NUM_EQUAL", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 실제 값과 기대하는 값이 다른지 검사하는 함수
#define ASSERT_NUM_NOT_EQUAL(actual, expected) ((actual) != (expected)) ? PRINT_SUCCESS("ASSERT_NUM_NOT_EQUAL", NUM2_FORMAT, actual, expected) : PRINT_FAIL("ASSERT_NUM_NOT_EQUAL", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 실제 값이 기대하는 값보다 작거나 같은지 검사하는 함수
#define ASSERT_NUM_LESS_EQUAL(actual, expected) ((actual) <= (expected)) ? PRINT_SUCCESS("ASSERT_NUM_LESS_EQUAL", NUM2_FORMAT, actual, expected) : PRINT_FAIL("ASSERT_NUM_LESS_EQUAL", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 실제 값이 기대하는 값보다 작은지 검사하는 함수
#define ASSERT_NUM_LESS_THAN(actual, expected) ((actual) < (expected)) ? PRINT_SUCCESS("ASSERT_NUM_LESS_THAN", NUM2_FORMAT, actual, expected) : PRINT_FAIL("ASSERT_NUM_LESS_THAN", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 실제 값이 기대하는 값보다 크거나 같은지 검사하는 함수
#define ASSERT_NUM_GREATER_EQUAL(actual, expected) ((actual) >= (expected)) ? PRINT_SUCCESS("ASSERT_NUM_GREATER_EQUAL", NUM2_FORMAT, actual, expected) : PRINT_FAIL("ASSERT_NUM_LESS_THAN", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 실제 값이 기대하는 값보다 큰지 검사하는 함수
#define ASSERT_NUM_GREATER_THAN(actual, expected) ((actual) > (expected)) ? PRINT_SUCCESS("ASSERT_NUM_GREATER_THAN", NUM2_FORMAT, actual, expected) : PRINT_FAIL("ASSERT_NUM_GREATER_EQUAL", __FILE__, __LINE__, ++failCount, NUM2_FORMAT, actual, expected)

// 지정한 숫자가 짝수인지 검사하는 함수
#define ASSERT_NUM_EVEN(actual) ((actual) % 2 == 0) ? PRINT_SUCCESS("ASSERT_NUM_EVEN", NUM1_FORMAT, actual) : PRINT_FAIL("ASSERT_NUM_EVEN", __FILE__, __LINE__, ++failCount, NUM1_FORMAT, actual)

// 지정한 숫자가 홀수인지 검사하는 함수
#define ASSERT_NUM_ODD(actual) ((actual) % 2 == 0) ? PRINT_SUCCESS("ASSERT_NUM_ODD", NUM1_FORMAT, actual) : PRINT_FAIL("ASSERT_NUM_ODD", __FILE__, __LINE__, ++failCount, NUM1_FORMAT, actual)

//////////////////////////////////////////////////////////////////////////////////
/// (ASSERT) String Macro Functions
/// 실패하면 현재 진행 중인 테스트가 종료된다.
//////////////////////////////////////////////////////////////////////////////////

// 실제 문자열과 기대하는 문자열이 같은지 검사하는 함수
#define ASSERT_STR_EQUAL(actual, expected) (strncmp((actual), (expected), (strlen(expected) + 1)) == 0) ? PRINT_SUCCESS("ASSERT_STR_EQUAL", STR_FORMAT, actual, expected) : PRINT_FAIL("ASSERT_STR_EQUAL", __FILE__, __LINE__, ++failCount, STR_FORMAT, actual, expected)

// 실제 문자열과 기대하는 문자열이 다른지 검사하는 함수
#define ASSERT_STR_NOT_EQUAL(actual, expected) (strncmp((actual), (expected), (strlen(expected) + 1)) != 0) ? PRINT_SUCCESS("ASSERT_STR_NOT_EQUAL", STR_FORMAT, actual, expected) : PRINT_FAIL("ASSERT_STR_NOT_EQUAL", __FILE__, __LINE__, ++failCount, STR_FORMAT, actual, expected)

//////////////////////////////////////////////////////////////////////////////////
/// Definitions
//////////////////////////////////////////////////////////////////////////////////

// 테스트와 관련된 구조체들을 미리 정의
struct _test_t;
struct _test_suit_t;

// 실행될 테스트 함수의 주소를 저장할 함수 포인터
typedef TestResult (*TestFunc)(struct _test_suit_t* testSuit);

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

void IncFailCountTestSuit(TestSuitPtr testSuit);
void SetExitTestSuit(TestSuitPtr testSuit);
void SetContinueTestSuit(TestSuitPtr testSuit);

void ProcessSuccessTest(const char *functionName, const char *format, ...);

#endif


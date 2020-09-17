#ifndef __TTLIB_H__
#define __TTLIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////////
/// Enum
//////////////////////////////////////////////////////////////////////////////////

typedef enum TEST_RESULT TEST_RESULT;
enum TEST_RESULT
{
	SUCCESS = 1,    // Test result is success.
	FATAL_FAIL,		// Test is failed and the test should be terminated.
	NON_FATAL_FAIL	// Test is failed but the test can continue.
};

//////////////////////////////////////////////////////////////////////////////////
/// Macros
//////////////////////////////////////////////////////////////////////////////////

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//////////////////////////////////////////////////////////////////////////////////
/// Control Macro Functions
//////////////////////////////////////////////////////////////////////////////////

#define DECLARE_TEST() static TestSuitPtr _testSuit = NULL;
#define TEST(C, T, F)                                         \
    void _##C##_##T(TestSuitPtr t) F;                         \
    void Test_##C##_##T()                                     \
    {                                                         \
        AddTest(_testSuit, (Test){#C, #T, _##C##_##T, 0, 0}); \
    }

#define CREATE_TESTSUIT()                               \
_testSuit = NewTestSuit();                              \
if (_testSuit == NULL) {                                \
    printf("failed to create a test suit instance!\n"); \
    exit(-1);                                           \
}

#define REGISTER_TESTS(X...)                           \
    _testSuit->initializers = (TestSuitInitializer[]){ \
        X,                                             \
        NULL};

#define RUN_ALL_TESTS() RunAllTests(_testSuit)
#define CLEAN_UP_TESTSUIT() DeleteTestSuit(&_testSuit)

//////////////////////////////////////////////////////////////////////////////////
/// Print Macro Functions
//////////////////////////////////////////////////////////////////////////////////

#define TEST_MESSAGE(msg, testResultType, functionName, fileName, lineNumber) printMessageHelper(functionName, fileName, lineNumber, msg, testResultType)

#define TEST_SUCCESS(msg) TEST_MESSAGE(msg, SUCCESS, NULL, NULL, 0)
#define TEST_FATAL_FAIL(msg, functionName, fileName, lineNumber) TEST_MESSAGE(msg, FATAL_FAIL, functionName, fileName, lineNumber)
#define TEST_NONFATAL_FAIL(msg, functionName, fileName, lineNumber) TEST_MESSAGE(msg, NON_FATAL_FAIL, functionName, fileName, lineNumber)

//////////////////////////////////////////////////////////////////////////////////
/// Compare Macro Functions
//////////////////////////////////////////////////////////////////////////////////

#define EXPECT_NUM_EQUAL(actual, expected) ((actual) == (expected)) ? (ProcessSuccessTestSuit(_testSuit, NULL)) : (ProcessFailTestSuit(_testSuit, #actual" 값과 "#expected" 값이 같지 않고 다름.", "EXPECT_NUM_EQUAL", __FILE__, __LINE__))
#define EXPECT_NUM_NOT_EQUAL(actual, expected) ((actual) != (expected)) ? (ProcessSuccessTestSuit(_testSuit, NULL)) : (ProcessFailTestSuit(_testSuit, #actual" 값과 "#expected" 값이 다르지 않고 같음.", "EXPECT_NUM_NOT_EQUAL", __FILE__, __LINE__))
#define EXPECT_NUM_LESS_EQUAL(actual, expected) ((actual) <= (expected)) ? (ProcessSuccessTestSuit(_testSuit, NULL)) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 큼.", "EXPECT_NUM_LESS_EQUAL", __FILE__, __LINE__))
#define EXPECT_NUM_LESS_THAN(actual, expected) ((actual) < (expected)) ? (ProcessSuccessTestSuit(_testSuit, NULL)) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 크거나 같음.", "EXPECT_NUM_LESS_THAN", __FILE__, __LINE__))
#define EXPECT_NUM_GREATER_EQUAL(actual, expected) ((actual) >= (expected)) ? (ProcessSuccessTestSuit(_testSuit, NULL)) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 작음.", "EXPECT_NUM_GREATER_EQUAL", __FILE__, __LINE__))
#define EXPECT_NUM_GREATER_THAN(actual, expected) ((actual) > (expected)) ? (ProcessSuccessTestSuit(_testSuit, NULL)) : (ProcessFailTestSuit(_testSuit, #actual" 값이 "#expected" 값보다 작거나 같음.", "EXPECT_NUM_GREATER_THAN", __FILE__, __LINE__))

#define EXPECT_STR_EQUAL(actual, expected) (strncmp((actual), (expected), (strlen(expected) + 1)) == 0) ? (ProcessSuccessTestSuit(_testSuit, NULL)) : (ProcessFailTestSuit(_testSuit, #actual" 문자열과 "#expected" 문자열이 같지 않고 다름.", "EXPECT_STR_EQUAL", __FILE__, __LINE__ ))
#define EXPECT_STR_NOT_EQUAL(actual, expected) (strncmp((actual), (expected), (strlen(expected) + 1)) != 0) ? (ProcessSuccessTestSuit(_testSuit, NULL)) : (ProcessFailTestSuit(_testSuit, #actual" 문자열과 "#expected" 문자열이 다르지 않고 같음.", "EXPECT_STR_NOT_EQUAL", __FILE__, __LINE__ ))

//////////////////////////////////////////////////////////////////////////////////
/// Definitions
//////////////////////////////////////////////////////////////////////////////////

struct _test_t;
struct _test_suit_t;

typedef void (*TestFunc)(struct _test_suit_t*);
typedef void (*TestSuitInitializer)();

// 사용자의 테스트 함수에 대한 정보를 관리하기 위한 구조체
typedef struct _test_t
{
    char *testCase;
    char *testName;
    TestFunc testFunc;
	int numberOfSuccessTestFunc;
	int numberOfFailTestFunc;
} Test, *TestPtr, **TestPtrContainer;

// 모든 사용자 테스트를 관리하기 위한 구조체
typedef struct _test_suit_t
{
    int numberOfTests;
	int numberOfSuccessTests;
	int numberOfFailTests;
	int totalNumOfSuccessTestFuncs;
	int totalNumOfFailTestFuncs;
    TestSuitInitializer *initializers;
    TestPtr tests;
	TestPtrContainer failTests;
} TestSuit, *TestSuitPtr, **TestSuitPtrContainer;

//////////////////////////////////////////////////////////////////////////////////
/// Local Functions
//////////////////////////////////////////////////////////////////////////////////

TestSuitPtr NewTestSuit();
void DeleteTestSuit(TestSuitPtrContainer testSuitContainer);

void AddTest(TestSuitPtr testSuit, Test test);
void RunAllTests(TestSuitPtr testSuit);

void ProcessSuccessTestSuit(TestSuitPtr testSuit, const char *msg);
void ProcessFailTestSuit(TestSuitPtr testSuit, const char *msg, const char *functionName, const char *fileName, int lineNumber);

//////////////////////////////////////////////////////////////////////////////////
/// Util Function
//////////////////////////////////////////////////////////////////////////////////

void printMessageHelper(const char *functionName, const char *file_name, int line_number, const char *msg, TEST_RESULT test_result_type);

#endif

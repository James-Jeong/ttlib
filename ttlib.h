#ifndef __TTLIB_H__
#define __TTLIB_H__

// -> enum
typedef enum TEST_RESULT TEST_RESULT;
enum TEST_RESULT
{
	SUCCESS = 1,    // Test result is success.
	FATAL_FAIL,		// Test is failed and the test should be terminated.
	NON_FATAL_FAIL	// Test is failed but the test can continue.
};

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define DECLARE_TEST() static TestSuitPtr _testSuit = NULL;
#define TEST(C, T, F)                                   \
    void _##C##_##T(TestSuitPtr t) F;                   \
    void Test_##C##_##T()                               \
    {                                                   \
        AddTest(_testSuit, (Test){#C, #T, _##C##_##T}); \
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

#define TEST_MESSAGE(msg, test_result_type) print_message_helper(__FILE__, __LINE__, msg, test_result_type)

#define TEST_SUCCESS(msg) TEST_MESSAGE(msg, SUCCESS)
#define TEST_FATAL_FAIL(msg) TEST_MESSAGE(msg, FATAL_FAIL)
#define TEST_NONFATAL_FAIL(msg) TEST_MESSAGE(msg, NON_FATAL_FAIL)

#define EXPECT_NUM_EQUAL(actual, expected) ((actual) == (expected)) ? TEST_SUCCESS(#actual" == "#expected) : TEST_NONFATAL_FAIL(#actual" != "#expected)
#define EXPECT_NUM_NOT_EQUAL(actual, expected) ((actual) != (expected)) ? TEST_SUCCESS(#actual" != "#expected) : TEST_NONFATAL_FAIL(#actual" == "#expected)

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
} Test, *TestPtr;

// 모든 사용자 테스트를 관리하기 위한 구조체
typedef struct _test_suit_t
{
    int numberOfTests;
    TestSuitInitializer *initializers;
    TestPtr tests;
} TestSuit, *TestSuitPtr, **TestSuitPtrContainer;


TestSuitPtr NewTestSuit();
void DeleteTestSuit(TestSuitPtrContainer testSuitContainer);

TestPtr AddTest(TestSuitPtr testSuit, Test test);
void RunAllTests(TestSuitPtr testSuit);

void print_message_helper(const char *file_name, int line_number, const char *msg, TEST_RESULT test_result_type);

#endif

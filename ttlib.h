#ifndef __TTLIB_H__
#define __TTLIB_H__

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


#endif

#include <gmock/gmock.h>
#include <gtest/gtest.h>
using namespace std::chrono_literals;

#include "Looper.h"

#define RUN_LOOPER(jthr, looper, sleep)                                     \
std::jthread jthr( [&](std::stop_token stoken) { looper.run( stoken ); } ); \
std::this_thread::sleep_for( sleep );                                       \
jthr.request_stop();

#define VERIFY_LOOP_GT(jthr, looper, testValue)  \
EXPECT_EQ( jthr.joinable(), true );              \
jthr.join();                                     \
EXPECT_GT( looper.counter(), testValue );

#define VERIFY_LOOP_EQ(jthr, looper, testValue)  \
EXPECT_EQ( jthr.joinable(), true );              \
jthr.join();                                     \
EXPECT_EQ( looper.counter(), testValue );

class SimpleLooper : public damn::Looper
{
public:
    inline static const int32_t k_someCounterValue { 666 };

    void loopTask()
    {
        ++m_counter;
        std::this_thread::sleep_for( 500ms );
    }

    int32_t counter() { return m_counter; }

protected:
    int32_t m_counter { 0 };
};

TEST(TestLooper, canBeInterrupted)
{
    SimpleLooper sl;

    RUN_LOOPER( jthr, sl, 1s )

    VERIFY_LOOP_GT( jthr, sl, 0 )
}

class MockSimpleLooper : public SimpleLooper
{
public:
    MockSimpleLooper()
    {
        using ::testing::Throw;
        ON_CALL(*this, beforeLoop()).WillByDefault(Throw(std::exception{"MockException"}));
    }

    MOCK_METHOD0(beforeLoop, void());
};

TEST(TestLooper, quitsWhenThrowsExceptionBeforeLoop)
{
    MockSimpleLooper mock;

    RUN_LOOPER( jthr, mock, 1s )

    VERIFY_LOOP_EQ( jthr, mock, 0 )
}

class BeforeLooper : public SimpleLooper
{
public:
    void beforeLoop() override
    {
        m_counter = k_someCounterValue;
    }

    void loopTask() override
    {
        std::this_thread::sleep_for( 100ms );
    }
};

TEST(TestLooper, executeWorkBeforeLoop)
{
    BeforeLooper bl;

    RUN_LOOPER( jthr, bl, 100ms )

    VERIFY_LOOP_EQ( jthr, bl, SimpleLooper::k_someCounterValue )
}

class AfterLooper : public SimpleLooper
{
public:
    void afterLoop() override
    {
        m_counter = k_someCounterValue;
    }

    void loopTask() override
    {
        std::this_thread::sleep_for( 100ms );
    }
};

TEST(TestLooper, executeWorkAfterLoop)
{
    AfterLooper al;

    RUN_LOOPER( jthr, al, 100ms )

    VERIFY_LOOP_EQ( jthr, al, SimpleLooper::k_someCounterValue )
}

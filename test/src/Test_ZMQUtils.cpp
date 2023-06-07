#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ZMQUtils.h"

inline damn::zmq_context_t ctx { 1 };

struct WrongFormatter
{
    static std::string format(std::string_view address, int32_t port)
    {
        return fmt::format( "{}:{}", address, port );
    }
};

TEST(TestSimpleMessage, string2message2string)
{
    const std::string str { "stork" };
    auto msg = damn::string2message( str );
    auto strConverted = damn::message2string( msg );

    EXPECT_EQ( strConverted, str );
}

TEST(TestFormatter, inprocFormatter)
{
    const std::string inprocAddress { "inproc://stork" };
    EXPECT_EQ( damn::InprocAddressFormatter::format( "stork", 0 ), inprocAddress );
}

TEST(TestFormatter, tcpFormatter)
{
    const std::string tcpAddress { "tcp://127.0.0.1:666" };
    EXPECT_EQ( damn::TcpAddressFormatter::format( "127.0.0.1", 666 ), tcpAddress );
}

TEST(TestSocketOperator, basicCreator)
{
    damn::SocketBaseCreator creator { ctx, zmq::socket_type::rep };
    auto socket = creator.apply();

    EXPECT_NE( socket.handle(), nullptr );
}

#define TEST_SOCKET_INITIALIZER(Initializer) \
damn::SocketBaseCreator creator { ctx, zmq::socket_type::rep }; \
Initializer<damn::InprocAddressFormatter> initializer { "stork", std::nullopt, creator };\
auto socket = initializer.apply();\
EXPECT_NE( socket.handle(), nullptr );

#define TEST_SOCKET_INITIALIZER_DEATH(Initializer) \
damn::SocketBaseCreator creator { ctx, zmq::socket_type::rep }; \
Initializer<WrongFormatter> initializer { "stork", std::nullopt, creator }; \
ASSERT_EXIT({ \
    initializer.apply(); \
    std::exit(0); \
}, ::testing::ExitedWithCode(3), ".*");

TEST(TestSocketOperator, connectInitializer)
{
    TEST_SOCKET_INITIALIZER(damn::ConnectInitializer)
}

TEST(TestSocketOperator, bindInitializer)
{
    TEST_SOCKET_INITIALIZER(damn::BindInitializer)
}

TEST(TestSocketOperator, connectInitializerTerminateWithInvalidAddress)
{
    TEST_SOCKET_INITIALIZER_DEATH(damn::ConnectInitializer)
}

TEST(TestSocketOperator, bindInitializerTerminateWithInvalidAddress)
{
    TEST_SOCKET_INITIALIZER_DEATH(damn::BindInitializer)
}

TEST(TestMakeSocket, canCreate)
{
    auto pub = damn::makeSocket<damn::BindInitializer<damn::InprocAddressFormatter>>( ctx, zmq::socket_type::pub, { "test", 0 } );
    auto sub = damn::makeSocket<damn::ConnectInitializer<damn::InprocAddressFormatter>>( ctx, zmq::socket_type::sub, { "test", 0 } );

    EXPECT_NE( pub.handle(), nullptr );
    EXPECT_NE( sub.handle(), nullptr );
}

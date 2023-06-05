#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ZMQUtils.h"

inline damn::zmq_context_t ctx { 1 };

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

TEST(TestSocketOperator, connectInitializer)
{
    damn::SocketBaseCreator creator { ctx, zmq::socket_type::rep };
    damn::ConnectInitializer<damn::InprocAddressFormatter> initializer { "stork", std::nullopt, creator };

    ASSERT_EXIT({
        auto socket = initializer.apply();
        EXPECT_NE( socket.handle(), nullptr );
        socket.close();

        std::exit(0);
    }, ::testing::ExitedWithCode(0), ".*");
}

TEST(TestSocketOperator, connectInitializerTerminateWithInvalidAddress)
{
    damn::SocketBaseCreator creator { ctx, zmq::socket_type::rep };
    damn::ConnectInitializer<damn::TcpAddressFormatter> initializer { "*", 5555, creator };

    ASSERT_EXIT({
        initializer.apply();

        std::exit(0);
    }, ::testing::ExitedWithCode(3), ".*");
}

TEST(TestSocketOperator, bindInitializer)
{
    damn::SocketBaseCreator creator { ctx, zmq::socket_type::rep };
    damn::BindInitializer<damn::InprocAddressFormatter> initializer { "stork", std::nullopt, creator };

    ASSERT_EXIT({
        auto socket = initializer.apply();
        EXPECT_NE( socket.handle(), nullptr );
        socket.close();

        std::exit(0);
    }, ::testing::ExitedWithCode(0), ".*");
}

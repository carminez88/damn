#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "EnumConverter.h"

constexpr int32_t k_undefined { -1 };
constexpr int32_t k_black { 0 };
constexpr int32_t k_white { 255 };

enum class SimpleEnum : int32_t {
    Undefined = k_undefined,
    Black = k_black,
    White = k_white,
};

TEST(TestSimpleEnum, operatorPlus)
{
    EXPECT_EQ( +SimpleEnum::Undefined, k_undefined );
    EXPECT_EQ( +SimpleEnum::Black, k_black );
    EXPECT_EQ( +SimpleEnum::White, k_white );
}

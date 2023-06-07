#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "CerealUtils.h"

using ::testing::Eq;

struct SimpleObject
{
    int32_t x;
    std::string y;
    double_t z;

    template<class Archive>
    void save(Archive& ar) const
    {
        ar(x, y, z);
    }

    template<class Archive>
    void load(Archive& ar)
    {
        ar(x, y, z);
    }
};

inline bool operator==(const SimpleObject& lhs, const SimpleObject& rhs)
{
    return ( lhs.x == rhs.x ) and ( lhs.y == rhs.y ) and ( lhs.z == rhs.z );
}

TEST(TestSimpleObject, packAndUnpack)
{
    SimpleObject gt { 5, "stork", 6.66F };

    auto packed = damn::pack<SimpleObject>( gt );

    EXPECT_EQ( packed.has_value(), true );
    EXPECT_EQ( packed.value().empty(), false );

    auto unpacked = damn::unpack<SimpleObject>( packed.value() );

    EXPECT_EQ( unpacked.has_value(), true );
    EXPECT_EQ( unpacked.value(), gt );
}

struct HeavyObject
{
    enum class HeavyEnum
    {
        Heavy0,
        Heavy1,
    };

    SimpleObject xyz;
    HeavyEnum he;
    int32_t i;

    template<class Archive>
    void save(Archive& ar) const
    {
        ar(xyz, he, i);
    }

    template<class Archive>
    void load(Archive& ar)
    {
        ar(xyz, he, i);
    }
};

inline bool operator==(const HeavyObject& lhs, const HeavyObject& rhs)
{
    return ( lhs.xyz == rhs.xyz ) and ( lhs.he == rhs.he ) and ( lhs.i == rhs.i );
}

TEST(TestHeavyObject, packAndUnpack)
{
    SimpleObject simple { 5, "stork", 6.66F };
    HeavyObject heavy { simple, HeavyObject::HeavyEnum::Heavy0, 666 };

    auto packed = damn::pack<HeavyObject>( heavy );

    EXPECT_EQ( packed.has_value(), true );
    EXPECT_EQ( packed.value().empty(), false );

    auto unpacked = damn::unpack<HeavyObject>( packed.value() );

    EXPECT_EQ( unpacked.has_value(), true );
    EXPECT_EQ( unpacked.value(), heavy );
}

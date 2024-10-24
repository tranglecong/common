#include "common/enum.hpp"

#include <array>
#include <gtest/gtest.h>
#include <string_view>

// Define some example enum values for testing
enum class Color
{
    Red = 1,
    Green = 2,
    Blue = 3,
    Unknown = 0
};

// Create an array of Enum<Color>
constexpr std::array<trlc::Enum<Color>, 4> colorEntries = {{{Color::Red, "Red"},
                                                            {Color::Green, "Green"},
                                                            {Color::Blue, "Blue"},
                                                            {Color::Unknown, "Unknown"}}};

// Create an EnumHolder instance for testing
namespace Policy = trlc::policy;
using ColorEnumHolder = trlc::EnumHolder<Color, colorEntries.size(), Policy::LinearSearchPolicy, Policy::CaseSensitiveStringSearchPolicy, Policy::UnknownPolicy>;

TEST(EnumHolderTest, FromValueTest)
{
    ColorEnumHolder holder{colorEntries};

    EXPECT_EQ(holder.fromValue(Color::Red).value, Color::Red);
    EXPECT_EQ(holder.fromValue(Color::Green).value, Color::Green);
    EXPECT_EQ(holder.fromValue(Color::Blue).value, Color::Blue);
    EXPECT_EQ(holder.fromValue(static_cast<Color>(5)).value, Color::Unknown); // Testing unknown value
}

TEST(EnumHolderTest, FromStringTest)
{
    ColorEnumHolder holder{colorEntries};

    EXPECT_EQ(holder.fromString("Red").value, Color::Red);
    EXPECT_EQ(holder.fromString("Green").value, Color::Green);
    EXPECT_EQ(holder.fromString("Blue").value, Color::Blue);
    EXPECT_EQ(holder.fromString("Purple").value, Color::Unknown); // Testing unknown string
}

TEST(EnumHolderTest, AllValuesTest)
{
    ColorEnumHolder holder{colorEntries};

    auto values = holder.allValues();
    EXPECT_EQ(values.size(), colorEntries.size());
    EXPECT_EQ(values[0].value, Color::Red);
    EXPECT_EQ(values[1].value, Color::Green);
    EXPECT_EQ(values[2].value, Color::Blue);
    EXPECT_EQ(values[3].value, Color::Unknown);
}

TEST(EnumHolderTest, CaseInsensitiveSearchTest)
{
    using InsensitiveHolder = trlc::EnumHolder<Color, colorEntries.size(), Policy::LinearSearchPolicy, Policy::CaseInsensitiveStringSearchPolicy, Policy::UnknownPolicy>;

    InsensitiveHolder holder{colorEntries};

    EXPECT_EQ(holder.fromString("red").value, Color::Red);
    EXPECT_EQ(holder.fromString("GREEN").value, Color::Green);
    EXPECT_EQ(holder.fromString("BluE").value, Color::Blue);
    EXPECT_EQ(holder.fromString("YELLOW").value, Color::Unknown); // Testing unknown string
}

TEST(EnumTest, ValueConversion)
{
    trlc::Enum<int> intEnum{42, "TestInt"};

    // Check value conversion
    int val = intEnum;
    EXPECT_EQ(val, 42);
}

TEST(EnumTest, EqualityOperator)
{
    trlc::Enum<int> enum1{1, "Enum1"};
    trlc::Enum<int> enum2{1, "Enum1"};
    trlc::Enum<int> enum3{2, "Enum2"};

    // Check equality
    EXPECT_TRUE(enum1 == enum2);
    EXPECT_FALSE(enum1 == enum3);
}

TEST(EnumTest, StringViewEqualityOperator)
{
    trlc::Enum<std::string_view> enum1{"value1", "Enum1"};
    trlc::Enum<std::string_view> enum2{"value1", "Enum1"};
    trlc::Enum<std::string_view> enum3{"value2", "Enum2"};

    // Check equality for std::string_view
    EXPECT_TRUE(enum1 == enum2);
    EXPECT_FALSE(enum1 == enum3);
}

TEST(EnumTest, StringEqualityOperator)
{
    trlc::Enum<std::string> enum1{"value1", "Enum1"};
    trlc::Enum<std::string> enum2{"value1", "Enum1"};
    trlc::Enum<std::string> enum3{"value2", "Enum2"};

    // Check equality for std::string
    EXPECT_TRUE(enum1 == enum2);
    EXPECT_FALSE(enum1 == enum3);
}

// Test constexpr functionality
constexpr trlc::Enum<int> constexprEnumInt{42, "TestInt"};
constexpr trlc::Enum<std::string_view> constexprEnumStrView{"value", "TestStrView"};
constexpr trlc::Enum<const char*> constexprEnumCharPtr{"value", "TestCharPtr"};

TEST(EnumTest, ConstexprValueConversion)
{
    // Check constexpr conversion of int
    constexpr int val = constexprEnumInt;
    static_assert(val == 42, "constexpr int value should be 42");
    EXPECT_EQ(val, 42);
}

TEST(EnumTest, ConstexprStringViewConversion)
{
    // Check constexpr conversion of string_view
    constexpr std::string_view strView = constexprEnumStrView;
    static_assert(strView == "value", "constexpr string_view should be 'TestStrView'");
    EXPECT_EQ(strView, "value");
}

TEST(EnumTest, ConstexprConstCharConversion)
{
    // Check constexpr conversion of const char*
    constexpr const char* charPtr = constexprEnumCharPtr;
    static_assert(charPtr != nullptr, "constexpr const char* should not be null");
    EXPECT_STREQ(charPtr, "value");
}

// Constexpr equality operator
TEST(EnumTest, ConstexprEqualityOperator)
{
    constexpr trlc::Enum<int> enum1{1, "Enum1"};
    constexpr trlc::Enum<int> enum2{1, "Enum1"};
    constexpr trlc::Enum<int> enum3{2, "Enum2"};

    // Check constexpr equality
    static_assert(enum1 == enum2, "enum1 should be equal to enum2");
    static_assert(!(enum1 == enum3), "enum1 should not be equal to enum3");
    EXPECT_TRUE(enum1 == enum2);
    EXPECT_FALSE(enum1 == enum3);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

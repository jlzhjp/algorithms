#include "string.hpp"
#include <gtest/gtest.h>

namespace alg::test {

std::string str1 = u8"abcdefghijklmnopqrstuvwxyz";
std::string str2 = u8"嘿!你好abc";
std::string str3 = u8"嘿!你好😂";
TEST(String_Utf8WalkLength, FullyAscii) { ASSERT_EQ(utf8_walk_length(str1), 26); }
TEST(String_Utf8WalkLength, WithChinese) { ASSERT_EQ(utf8_walk_length(str2), 7); }
TEST(String_Utf8WalkLength, WithEmoji) { ASSERT_EQ(utf8_walk_length(str3), 5); }

TEST(String_Utf8CharAt, FullyAscii) { ASSERT_EQ(utf8_char_at(str1, 1), u8"b"); }
TEST(String_Utf8CharAt, WithChinese) { ASSERT_EQ(utf8_char_at(str2, 3), u8"好"); }
TEST(String_Utf8CharAt, WithEmoji) { ASSERT_EQ(utf8_char_at(str3, 4), u8"😂"); }
TEST(String_Utf8CharAt, NonStardard) {
    std::string str = "0";
    str[0] = 0xBF;
    ASSERT_THROW(utf8_char_at(str, 0), std::invalid_argument);
}
TEST(String_Utf8CharAt, OutOfRange) { ASSERT_THROW(utf8_char_at(str3, 5), std::out_of_range); }

}  // namespace alg::test
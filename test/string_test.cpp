#include <gtest/gtest.h>
#include "string.hpp"

namespace alg::test {

std::string str1 = "abcdefghijklmnopqrstuvwxyz";
std::string str2 = "嘿!你好abc";
std::string str3 = "嘿!你好😃";
TEST(String_Utf8WalkLength, FullyAscii) {
    ASSERT_EQ(utf8_walk_length(str1), 26);
}
TEST(String_Utf8WalkLength, WithChinese) {
    ASSERT_EQ(utf8_walk_length(str2), 7);
}
TEST(String_Utf8WalkLength, WithEmoji) {
    ASSERT_EQ(utf8_walk_length(str3), 5);
}

TEST(String_Utf8CharAt, FullyAscii) {
    ASSERT_EQ(utf8_char_at(str1, 1), "b");
}
TEST(String_Utf8CharAt, WithChinese) {
    ASSERT_EQ(utf8_char_at(str2, 3), "好");
}
TEST(String_Utf8CharAt, WithEmoji) {
    ASSERT_EQ(utf8_char_at(str3, 4), "😃");
}
TEST(String_Utf8CharAt, NonStardard) {
    std::string str = "0";
    str[0] = 0xBF;
    ASSERT_THROW(utf8_char_at(str, 0), std::invalid_argument);
}
TEST(String_Utf8CharAt, OutOfRange) {
    ASSERT_THROW(utf8_char_at(str3, 5), std::out_of_range);
}

}
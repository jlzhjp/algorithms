#include "string.hpp"

#include <stdexcept>

namespace alg {

// BIN  HEX
// 0000 0   0101 5  1010 A  1111 F
// 0001 1   0110 6  1011 B
// 0010 2   0111 7  1100 C
// 0011 3   1000 8  1101 D
// 0100 4   1001 9  1110 E

// 7  U+0000~U+007F        1 0xxxxxxx
// 11 U+0080~U+07FF        2 110xxxxx 10xxxxxx
// 16 U+0800~U+FFFF        3 1110xxxx 10xxxxxx 10xxxxxx
// 21 U+10000~U+1FFFFF     4 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
// 26 U+200000~U+3FFFFFF   5 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
// 31 U+4000000~U+7FFFFFFF 6 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

std::string::size_type utf8_walk_length(const std::string &str) {
    std::string::size_type len = 0;
    for (char b : str) {
        if ((b & 0xC0) != 0x80) ++len;
    }
    return len;
}
std::string utf8_char_at(const std::string &str, std::string::size_type idx) {
    using size_type = std::string::size_type;
    using const_iterator = std::string::const_iterator;
    size_type n = 0;
    short current_char_size = -1;
    for (const_iterator iter = str.cbegin(); iter < str.cend();) {
        if ((*iter & 0x80) == 0x00) {
            current_char_size = 1;
        } else if ((*iter & 0xE0) == 0xC0) {
            current_char_size = 2;
        } else if ((*iter & 0xF0) == 0xE0) {
            current_char_size = 3;
        } else if ((*iter & 0xF8) == 0xF0) {
            current_char_size = 4;
        } else if ((*iter & 0xFC) == 0xF8) {
            current_char_size = 5;
        } else if ((*iter & 0xFE) == 0xFC) {
            current_char_size = 6;
        } else {
            throw std::invalid_argument("Not a standard UTF-8 string.");
        }
        if (n == idx) {
            return std::string(iter, iter + current_char_size);
        }
        ++n;
        iter += current_char_size;
    }
    throw std::out_of_range("Index out of range.");
}

}  // namespace alg

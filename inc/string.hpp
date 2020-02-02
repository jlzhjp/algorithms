#include <string>

namespace alg {
std::string::size_type utf8_walk_length(const std::string &str);
std::string utf8_char_at(const std::string &str, std::string::size_type idx);
}  // namespace alg

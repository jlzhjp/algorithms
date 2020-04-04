#include "union_find/quick_union.hpp"

#include <numeric>

namespace alg {

QuickUnion::QuickUnion(size_t n) : _map(n) {
    std::iota(_map.begin(), _map.end(), 0);
}

void QuickUnion::connect(size_t n1, size_t n2) { _map[n1] = n2; }

bool QuickUnion::is_connected(size_t n1, size_t n2) {
    return find_root(n1) == find_root(n2);
}

int QuickUnion::find_root(int i) {
    while (_map[i] != i) i = _map[i];
    return i;
}

}  // namespace alg

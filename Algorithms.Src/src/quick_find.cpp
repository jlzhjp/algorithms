#include "union_find/quick_find.hpp"

namespace alg {

QuickFind::QuickFind(size_t n) : _map(n) {
    std::iota(_map.begin(), _map.end(), 0);
}
void QuickFind::connect(size_t n1, size_t n2) {
    int v1 = _map[n1];
    int v2 = _map[n2];
    if (v1 == v2) return;
    for (int &val : _map) {
        if (val == v1) val = v2;
    }
}
bool QuickFind::is_connected(size_t n1, size_t n2) {
    assert(n1 >= 0 && n1 < _map.size());
    assert(n2 >= 0 && n2 < _map.size());
    return _map[n1] == _map[n2];
}

}  // namespace alg

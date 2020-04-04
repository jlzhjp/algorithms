#pragma once

#include <cassert>
#include <numeric>
#include <vector>

namespace alg {

class QuickFind {
public:
    QuickFind(size_t n);

public:
    void connect(size_t n1, size_t n2);
    bool is_connected(size_t n1, size_t n2);

private:
    std::vector<int> _map;
};

}  // namespace alg

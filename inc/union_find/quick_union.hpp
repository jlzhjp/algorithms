#pragma once

#include <cstdlib>
#include <vector>

namespace alg {

class QuickUnion {
public:
    QuickUnion(size_t n);

public:
    void connect(size_t n1, size_t n2);
    bool is_connected(size_t n1, size_t n2);

private:
    int find_root(int i);

private:
    std::vector<int> _map;
};

}  // namespace alg

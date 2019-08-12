#pragma once

#include "resizing_array.hpp"

namespace alg {

template <typename V, typename A = std::allocator<V>>
class Vector : public ResizingArray<V, A> {};

}  // namespace alg

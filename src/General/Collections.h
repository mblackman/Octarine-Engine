#pragma once
#include <algorithm>
#include <vector>

namespace Collections {
template <typename T>
/**
 * Efficiently removes an item from a vector without preserving order.
 * It finds the item, swaps it with the last element, and then pops the back.
 * This avoids the costly O(N) shift of elements that `vector::erase` can cause.
 *
 * @tparam T The type of elements in the vector.
 * @param vec The vector to remove the item from.
 * @param item The item to remove.
 * @return True if the item was found and removed, false otherwise.
 */

bool SwapAndPop(std::vector<T>& vec, const T& item) {
  const auto it = std::find(vec.begin(), vec.end(), item);
  if (it != vec.end()) {
    *it = std::move(vec.back());
    vec.pop_back();
    return true;
  }
  return false;
}

}  // namespace Collections

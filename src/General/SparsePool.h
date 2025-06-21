#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class IPool {
 public:
  IPool() = default;
  virtual ~IPool() = default;

  IPool(const IPool&) = delete;
  IPool& operator=(const IPool&) = delete;
  IPool(IPool&&) = delete;
  IPool& operator=(IPool&&) = delete;

  virtual void Remove(int id) = 0;
};

/**
 * @brief A sparse-set pool that provides O(1) insertion, deletion, and access
 * via an integer ID.
 *
 * This pool stores objects of type T in a contiguous block of memory.
 * It uses a "swap and pop" strategy for removals to avoid memory fragmentation
 * and maintain data locality, which is ideal for performance-critical applications
 * like games. The order of elements is not guaranteed.
 *
 * @tparam T The type of object to store in the pool.
 */
template <typename T>
class SparsePool final : public IPool {
 private:
  std::vector<T> data_;

  std::unordered_map<int, size_t> id_to_index_;
  std::unordered_map<size_t, int> index_to_id_;

 public:
  /**
   * @brief Constructs the Pool.
   * @param initial_capacity The initial amount of memory to reserve.
   * This helps avoid reallocations on initial insertions.
   */
  explicit SparsePool(const size_t initial_capacity = 100) { data_.reserve(initial_capacity); }

  /**
   * @brief Checks if the pool contains an element with the given ID.
   */
  [[nodiscard]] bool Contains(const int id) const { return id_to_index_.count(id) > 0; }

  /**
   * @brief Checks if the pool is empty.
   */
  [[nodiscard]] bool IsEmpty() const { return data_.empty(); }

  /**
   * @brief Gets the number of elements currently in the pool.
   */
  [[nodiscard]] size_t GetSize() const { return data_.size(); }

  /**
   * @brief Removes all elements from the pool and clears all memory.
   */
  void Clear() {
    data_.clear();
    id_to_index_.clear();
    index_to_id_.clear();
  }

  /**
   * @brief Adds a new element or updates an existing one.
   * @param id The ID of the element.
   * @param value The object to add or use for the update.
   */
  void Set(const int id, T value) {
    if (const auto it = id_to_index_.find(id); it != id_to_index_.end()) {
      data_[it->second] = std::move(value);
    } else {
      const size_t new_index = data_.size();

      id_to_index_[id] = new_index;
      index_to_id_[new_index] = id;

      data_.emplace_back(std::move(value));
    }
  }

  /**
   * @brief Removes an element from the pool using the "swap and pop" method.
   * This is an O(1) operation but does not preserve the order of elements.
   * @param id The ID of the element to remove.
   */
  void Remove(const int id) override {
    const auto it = id_to_index_.find(id);
    if (it == id_to_index_.end()) {
      // The ID doesn't exist, so there's nothing to do.
      return;
    }

    const size_t index_to_remove = it->second;

    const size_t last_index = data_.size() - 1;

    if (index_to_remove != last_index) {
      data_[index_to_remove] = std::move(data_[last_index]);

      const int id_of_last_element = index_to_id_.at(last_index);
      id_to_index_[id_of_last_element] = index_to_remove;
      index_to_id_[index_to_remove] = id_of_last_element;
    }

    id_to_index_.erase(id);
    index_to_id_.erase(last_index);

    data_.pop_back();
  }

  /**
   * @brief Gets a reference to an element by its ID.
   * @throws std::runtime_error if the ID is not found.
   */
  T& Get(const int id) {
    const auto it = id_to_index_.find(id);
    if (it == id_to_index_.end()) {
      throw std::runtime_error("Pool::Get Error: Element not found with id: " + std::to_string(id));
    }
    return data_[it->second];
  }

  /**
   * @brief Gets a const reference to an element by its ID.
   * @throws std::runtime_error if the ID is not found.
   */
  const T& Get(const int id) const {
    const auto it = id_to_index_.find(id);
    if (it == id_to_index_.end()) {
      throw std::runtime_error("Pool::Get Error: Element not found with id: " + std::to_string(id));
    }
    return data_[it->second];
  }
};
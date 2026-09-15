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

// Sparse-set pool providing O(1) lookup, insertion, and swap-and-pop removal
// while storing elements contiguously.
template <typename T>
class SparsePool final : public IPool {
 private:
  std::vector<T> data_;

  std::unordered_map<int, size_t> id_to_index_;
  std::unordered_map<size_t, int> index_to_id_;

 public:
  explicit SparsePool(const size_t initial_capacity = 100) { data_.reserve(initial_capacity); }

  [[nodiscard]] bool Contains(const int id) const { return id_to_index_.count(id) > 0; }
  [[nodiscard]] bool IsEmpty() const { return data_.empty(); }
  [[nodiscard]] size_t GetSize() const { return data_.size(); }

  void Clear() {
    data_.clear();
    id_to_index_.clear();
    index_to_id_.clear();
  }

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

  // Swap-and-pop removal: O(1) unordered deletion.
  void Remove(const int id) override {
    const auto it = id_to_index_.find(id);
    if (it == id_to_index_.end()) {
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

  T& Get(const int id) {
    const auto it = id_to_index_.find(id);
    if (it == id_to_index_.end()) {
      throw std::runtime_error("Pool::Get Error: Element not found with id: " + std::to_string(id));
    }
    return data_[it->second];
  }

  const T& Get(const int id) const {
    const auto it = id_to_index_.find(id);
    if (it == id_to_index_.end()) {
      throw std::runtime_error("Pool::Get Error: Element not found with id: " + std::to_string(id));
    }
    return data_[it->second];
  }
};
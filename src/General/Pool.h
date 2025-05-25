#pragma once

#include <vector>

class IPool {
   public:
    virtual ~IPool() = default;
    virtual void Remove(int id) = 0;
};

/**
 * A contiguous set of object of type T.
 */
template <typename T>
class Pool : public IPool {
   private:
    std::vector<T> data_;
    size_t size_;

    std::unordered_map<int, int> index_to_ids_;
    std::unordered_map<int, int> id_to_indexes_;

   public:
    Pool(size_t size = 100) : size_(0), index_to_ids_(), id_to_indexes_() {
        Resize(size);
    }

    bool IsEmpty() const {
        return size_ == 0;
    }

    size_t GetSize() const {
        return size_;
    }

    void Resize(size_t size) {
        data_.resize(size);
    }

    void Clear() {
        data_.clear();
        size_ = 0;
        index_to_ids_.clear();
        id_to_indexes_.clear();
    }

    void Set(int id, const T value) {
        if (id_to_indexes_.find(id) != id_to_indexes_.end()) {
            int index = id_to_indexes_[id];
            data_[index] = value;
        } else {
            size_t index = size_;
            id_to_indexes_.emplace(id, index);
            index_to_ids_.emplace(index, id);

            if (index >= data_.capacity()) {
                Resize(size_ * 2);
            }

            data_[index] = value;
            size_++;
        }
    }

    void Remove(int id) override {
        auto index = id_to_indexes_.find(id);
        if (index == id_to_indexes_.end()) {
            return;
        }

        int indexOfRemoved = index->second;
        int indexOfLast = size_ - 1;
        data_[indexOfRemoved] = data_[indexOfLast];

        int entityIdOfLastElement = index_to_ids_[indexOfLast];
        id_to_indexes_[entityIdOfLastElement] = indexOfRemoved;
        index_to_ids_[indexOfRemoved] = entityIdOfLastElement;

        id_to_indexes_.erase(id);
        index_to_ids_.erase(indexOfLast);

        size_--;
    }

    T& Get(int id) {
        auto index = id_to_indexes_.find(id);
        if (index == id_to_indexes_.end()) {
            throw std::runtime_error("Element not found with id: " + std::to_string(id));
        }

        return static_cast<T&>(data_[index->second]);
    }

    T& operator[](unsigned int index) {
        return data_[index];
    }
};
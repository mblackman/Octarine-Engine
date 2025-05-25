#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

#include "./RenderKey.h"

class RenderQueue {
   public:
    using value_type = RenderKey;
    using const_iterator = typename std::vector<RenderKey>::const_iterator;

    RenderQueue(size_t initial_capacity = 1024) {
        render_keys_.reserve(initial_capacity);
    }

    ~RenderQueue() = default;

    RenderQueue(const RenderQueue&) = delete;
    RenderQueue& operator=(const RenderQueue&) = delete;

    RenderQueue(RenderQueue&&) = default;
    RenderQueue& operator=(RenderQueue&&) = default;

    void AddRenderKey(const RenderKey& key) {
        render_keys_.push_back(key);
    }

    void AddRenderKey(RenderKey&& key) {
        render_keys_.push_back(std::move(key));
    }

    void Clear() {
        render_keys_.clear();
    }

    void Sort() {
        std::sort(render_keys_.begin(), render_keys_.end());
    }

    const_iterator begin() const noexcept {
        return render_keys_.cbegin();
    }

    const_iterator end() const noexcept {
        return render_keys_.cend();
    }

    const_iterator cbegin() const noexcept {
        return render_keys_.cbegin();
    }

    const_iterator cend() const noexcept {
        return render_keys_.cend();
    }

    bool IsEmpty() const noexcept {
        return render_keys_.empty();
    }

    size_t Size() const noexcept {
        return render_keys_.size();
    }

   private:
    std::vector<RenderKey> render_keys_;
};
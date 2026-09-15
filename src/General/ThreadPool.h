#pragma once

#include <algorithm>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

// Persistent worker pool sized to hardware concurrency.
class ThreadPool {
 public:
  static ThreadPool& Instance() {
    static ThreadPool inst(std::max<size_t>(1u, std::thread::hardware_concurrency()));
    return inst;
  }

  [[nodiscard]] size_t Size() const { return workers_.size(); }

  // Runs fn(batchIndex, begin, end) over contiguous sub-ranges in parallel across the pool.
  // fn must be thread-safe for disjoint writes and must not be called from a worker thread.
  template <typename Fn>
  static void ParallelChunks(size_t count, Fn&& fn) {
    if (count == 0) return;

    const size_t numBatches = std::min(count, Instance().Size());
    if (numBatches <= 1) {
      fn(static_cast<size_t>(0), static_cast<size_t>(0), count);
      return;
    }

    // Synchronizes batch completion before destroying barrier.
    struct Barrier {
      size_t remaining;
      std::mutex mutex;
      std::condition_variable cv;
      explicit Barrier(size_t n) : remaining(n) {}
      void Signal() {
        std::lock_guard<std::mutex> lk(mutex);
        if (--remaining == 0) cv.notify_one();
      }
      void Wait() {
        std::unique_lock<std::mutex> lk(mutex);
        cv.wait(lk, [this] { return remaining == 0; });
      }
    } barrier(numBatches - 1);

    const size_t per = (count + numBatches - 1) / numBatches;
    for (size_t b = 0; b < numBatches - 1; ++b) {
      const size_t begin = b * per;
      const size_t end = std::min(begin + per, count);
      Instance().Submit([&fn, &barrier, b, begin, end] {
        fn(b, begin, end);
        barrier.Signal();
      });
    }

    // Caller runs the final batch inline rather than sitting idle, then waits for the dispatched
    // ones. The barrier count equals the number of Submits regardless of whether any range is
    // empty, so there is no deadlock even on a ragged final split.
    const size_t inlineBegin = (numBatches - 1) * per;
    if (inlineBegin < count) {
      fn(numBatches - 1, inlineBegin, count);
    }
    barrier.Wait();
  }

  void Submit(std::function<void()> task) {
    {
      std::lock_guard<std::mutex> lk(jobs_mutex_);
      jobs_.push(std::move(task));
    }
    jobs_cv_.notify_one();
  }

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  ~ThreadPool() {
    {
      std::lock_guard<std::mutex> lk(jobs_mutex_);
      stop_ = true;
    }
    jobs_cv_.notify_all();
    for (auto& t : workers_) {
      if (t.joinable()) t.join();
    }
  }

 private:
  explicit ThreadPool(size_t numThreads) {
    workers_.reserve(numThreads);
    for (size_t i = 0; i < numThreads; ++i) {
      workers_.emplace_back([this] { WorkerLoop(); });
    }
  }

  void WorkerLoop() {
    for (;;) {
      std::function<void()> job;
      {
        std::unique_lock<std::mutex> lk(jobs_mutex_);
        jobs_cv_.wait(lk, [this] { return stop_ || !jobs_.empty(); });
        if (stop_ && jobs_.empty()) return;
        job = std::move(jobs_.front());
        jobs_.pop();
      }
      job();
    }
  }

  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> jobs_;
  std::mutex jobs_mutex_;
  std::condition_variable jobs_cv_;
  bool stop_ = false;
};

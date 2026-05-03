#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

// Persistent worker pool. Replaces per-frame std::async(launch::async) calls so
// hot per-frame parallel sections (e.g. ArchetypeQuery::ParallelForEach) skip
// thread startup. Process-wide singleton sized to hardware_concurrency().
class ThreadPool {
 public:
  static ThreadPool& Instance() {
    static ThreadPool inst(std::max<size_t>(1u, std::thread::hardware_concurrency()));
    return inst;
  }

  [[nodiscard]] size_t Size() const { return workers_.size(); }

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

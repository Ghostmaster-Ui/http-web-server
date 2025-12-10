#include "thread_pool.h"

namespace http {

ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
    for (size_t i = 0; i < num_threads; i++) {
        workers_.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock lock(mutex_);
                    cv_.wait(lock, [this]() {
                        return stop_ || !tasks_.empty();
                    });
                    if (stop_ && tasks_.empty()) return;
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    stop_ = true;
    cv_.notify_all();
    for (auto& w : workers_) w.join();
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock lock(mutex_);
        tasks_.push(std::move(task));
    }
    cv_.notify_one();
}

size_t ThreadPool::queue_size() const {
    std::unique_lock lock(mutex_);
    return tasks_.size();
}

} // namespace http

#pragma once

#include <atomic>
#include <future>
#include <memory>
#include <vector>
#include <thread>

namespace braid::thread {

template<typename Iterator>
void waitForFutures(Iterator begin, Iterator end) {
  std::mutex mutex;

  std::condition_variable condition;

  std::vector<std::shared_future<void>> futures;

  std::atomic<int> count(std::distance(begin, end));

  for (auto futureIterator = begin; futureIterator != end; ++futureIterator) {
    futures.push_back(std::async(std::launch::async,
      [=, &count, &condition, &mutex]() {
        futureIterator->get(); // wait for result

        if (count-- == 1) {
          std::lock_guard<std::mutex> lock(mutex);

          condition.notify_all();
        }
      }));
  }

  std::unique_lock<std::mutex> lock(mutex);

  condition.wait(lock);
}

} // namespace braid::thread

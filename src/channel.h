#pragma once

#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>

template <typename T> class channel {
    std::shared_ptr<std::mutex> lock;
    std::shared_ptr<std::deque<T>> buffer;
    std::shared_ptr<std::condition_variable> waker;

  public:
    channel() {
        lock = std::make_shared<std::mutex>();
        buffer = std::make_shared<std::deque<T>>();
        waker = std::make_shared<std::condition_variable>();
    };

    T recv() {
        std::unique_lock<std::mutex> guard(*lock);
        if (buffer->size() == 0) {
            waker->wait(guard, [this]() { return buffer->size() != 0; });
        }
        T front = buffer->front();
        buffer->pop_front();
        return front;
    }

    void send(T value) {
        std::unique_lock<std::mutex> guard(*lock);
        buffer->push_back(value);
        waker->notify_one();
    }
};

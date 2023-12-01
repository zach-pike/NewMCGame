#pragma once

#include <thread>
#include <queue>

template <typename T>
class TSQueue {
private:
    std::queue<T> messages;
    std::mutex messagesLock;
public:
    TSQueue();
    ~TSQueue();

    void addMessage(T msg);

    [[nodiscard]]
    T getMessage();

    bool messageAvailable();

    std::size_t getMessageCount() const;
};

#ifndef TSQUEUE_IMPL
#define TSQUEUE_IMPL

template<typename T>
TSQueue<T>::TSQueue() {}

template<typename T>
TSQueue<T>::~TSQueue() {}

template<typename T>
void TSQueue<T>::addMessage(T message) {
    messagesLock.lock();
    messages.push(message);
    messagesLock.unlock();
}

template<typename T>
T TSQueue<T>::getMessage() {
    messagesLock.lock();
    T v = messages.front();
    messages.pop();
    messagesLock.unlock();
    return v;
}

template<typename T>
bool TSQueue<T>::messageAvailable() {
    return messages.size() > 0;
}

template<typename T>
std::size_t TSQueue<T>::getMessageCount() const {
    return messages.size();
}

#endif
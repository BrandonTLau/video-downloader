#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>

using namespace std;

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads);
    void enqueue(function<void()> task);
    void waitAll();
    ~ThreadPool();

private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex queueMutex;
    condition_variable cv;
    condition_variable doneCV;
    int activeTasks = 0;
    bool stop = false;
};
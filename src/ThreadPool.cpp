#include "ThreadPool.h"

using namespace std;

ThreadPool::ThreadPool(size_t numThreads) {
    for (size_t i = 0; i < numThreads; i++) {
        workers.emplace_back([this] {
            while (true) {
                function<void()> task;
                {
                    unique_lock<mutex> lock(queueMutex);
                    cv.wait(lock, [this] { return stop || !tasks.empty(); });
                    if (stop && tasks.empty()) return;
                    task = tasks.front();
                    tasks.pop();
                    activeTasks++;
                }
                task();
                {
                    unique_lock<mutex> lock(queueMutex);
                    activeTasks--;
                    doneCV.notify_all();
                }
            }
        });
    }
}

void ThreadPool::enqueue(function<void()> task) {
    unique_lock<mutex> lock(queueMutex);
    tasks.push(task);
    cv.notify_one();
}

void ThreadPool::waitAll() {
    unique_lock<mutex> lock(queueMutex);
    doneCV.wait(lock, [this] { return tasks.empty() && activeTasks == 0; });
}

ThreadPool::~ThreadPool() {
    {
        unique_lock<mutex> lock(queueMutex);
        stop = true;
    }
    cv.notify_all();
    for (auto& worker : workers)
        worker.join();
}
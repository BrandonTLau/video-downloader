#pragma once
#include <atomic>
#include <iostream>
#include <chrono>

using namespace std;

class ProgressTracker {
public:
    ProgressTracker(size_t totalBytes);
    void update(size_t bytesDownloaded);
    void printBar();

private:
    size_t totalBytes;
    atomic<size_t> downloadedBytes;
    chrono::steady_clock::time_point startTime;
};
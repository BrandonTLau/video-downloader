#include "ProgressTracker.h"

using namespace std;

ProgressTracker::ProgressTracker(size_t total) {
    totalBytes = total;
    downloadedBytes = 0;
    startTime = chrono::steady_clock::now();
}

void ProgressTracker::update(size_t bytes) {
    downloadedBytes += bytes;
    printBar();
}

void ProgressTracker::printBar() {
    int percent = (int)(downloadedBytes * 100 / totalBytes);
    int filled = percent / 2;

    auto now = chrono::steady_clock::now();
    double elapsed = chrono::duration<double>(now - startTime).count();
    double speedMB = (downloadedBytes / (1024.0 * 1024.0)) / (elapsed > 0 ? elapsed : 1);

    string bar = "[" + string(filled, '=') + string(50 - filled, ' ') + "]";
    cout << "\r" << bar << " " << percent << "% " << speedMB << " MB/s" << flush;
}
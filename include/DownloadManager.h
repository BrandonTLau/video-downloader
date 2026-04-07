#pragma once
#include <string>
#include "ThreadPool.h"
#include "ProgressTracker.h"
#include "ResumeState.h"

using namespace std;

class DownloadManager {
public:
    DownloadManager(const string& url, const string& outputPath, int numThreads);
    void start();
    bool hasResumeState();

private:
    string url;
    string outputPath;
    int numThreads;
    size_t getRemoteFileSize();
    void splitAndDispatch(size_t fileSize);
    void mergeChunks(int numChunks);
    bool isYouTubeURL();
    void downloadWithYtDlp();
    bool isPlaylist();
};
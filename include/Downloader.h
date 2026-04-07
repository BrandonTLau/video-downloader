#pragma once
#include <string>
#include <fstream>
#include <curl/curl.h>

using namespace std;

class Downloader {
public:
    bool fetchChunk(const string& url, const string& filePath,
                    size_t startByte, size_t endByte);

private:
    static size_t writeCallback(void* ptr, size_t size, size_t nmemb, void* userdata);
};
#include "Downloader.h"
#include <fstream>
#include <iostream>

using namespace std;

size_t Downloader::writeCallback(void* ptr, size_t size, size_t nmemb, void* userdata) {
    ofstream* file = (ofstream*)userdata;
    file->write((char*)ptr, size * nmemb);
    return size * nmemb;
}

bool Downloader::fetchChunk(const string& url, const string& filePath,
                             size_t startByte, size_t endByte) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    ofstream file(filePath, ios::binary);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filePath << "\n";
        return false;
    }

    string range = to_string(startByte) + "-" + to_string(endByte);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_RANGE, range.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 120L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    file.close();

    return res == CURLE_OK;
}
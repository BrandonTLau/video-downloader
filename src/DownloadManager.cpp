#include "DownloadManager.h"
#include "Downloader.h"
#include <iostream>
#include <fstream>
#include <curl/curl.h>

using namespace std;

DownloadManager::DownloadManager(const string& url, const string& outputPath, int numThreads)
    : url(url), outputPath(outputPath), numThreads(numThreads) {}

static size_t discardData(void* ptr, size_t size, size_t nmemb, void* userdata) {
    return size * nmemb;
}

static size_t headerCallback(void* ptr, size_t size, size_t nmemb, void* userdata) {
    string header((char*)ptr, size * nmemb);
    size_t* fileSize = (size_t*)userdata;

    // Content-Range: bytes 0-0/158008374
    if (header.find("Content-Range:") != string::npos) {
        size_t slash = header.find('/');
        if (slash != string::npos) {
            *fileSize = stoull(header.substr(slash + 1));
        }
    }
    return size * nmemb;
}

size_t DownloadManager::getRemoteFileSize() {
    CURL* curl = curl_easy_init();
    curl_off_t fileSize = -1;

    if (curl) {
        // Try HEAD first
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discardData);
        curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &fileSize);

        // If HEAD failed try GET
        if (fileSize <= 0) {
            curl_easy_reset(curl);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discardData);
            curl_easy_perform(curl);
            curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &fileSize);
        }

        curl_easy_cleanup(curl);
    }

    if (fileSize <= 0) return 0;
    return (size_t)fileSize;
}

bool DownloadManager::hasResumeState() {
    ifstream file(outputPath + ".resume");
    return file.is_open();
}

void DownloadManager::mergeChunks(int numChunks) {
    cout << "\nMerging chunks...\n";
    ofstream output(outputPath, ios::binary);
    for (int i = 0; i < numChunks; i++) {
        string chunkPath = outputPath + ".part" + to_string(i);
        ifstream chunk(chunkPath, ios::binary);
        output << chunk.rdbuf();
        chunk.close();
        remove(chunkPath.c_str());
    }
    output.close();
    cout << "Merge complete!\n";
}

void DownloadManager::splitAndDispatch(size_t fileSize) {
    ThreadPool pool(numThreads);
    ProgressTracker progress(fileSize);
    ResumeState resumeState(outputPath);
    resumeState.load();

    size_t chunkSize = fileSize / numThreads;

    for (int i = 0; i < numThreads; i++) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? fileSize - 1 : start + chunkSize - 1;

        if (resumeState.isChunkDone(i)) {
            cout << "Skipping chunk " << i << "\n";
            progress.update(end - start + 1);
            continue;
        }

        string chunkPath = outputPath + ".part" + to_string(i);
        pool.enqueue([=, &progress, &resumeState]() {
            Downloader d;
            bool ok = false;
            for (int attempt = 1; attempt <= 3; attempt++) {
                ok = d.fetchChunk(url, chunkPath, start, end);
                if (ok) break;
                cout << "\nChunk " << i << " failed, retry " << attempt << "/3\n";
            }
            if (ok) {
                resumeState.markChunkDone(i);
                progress.update(end - start + 1);
            } else {
                cout << "\nChunk " << i << " failed after 3 attempts!\n";
            }
        });
    }

    pool.waitAll();
    mergeChunks(numThreads);
}

bool DownloadManager::isYouTubeURL() {
    return url.find("youtube.com") != string::npos ||
           url.find("youtu.be") != string::npos;
}

bool DownloadManager::isPlaylist() {
    return url.find("playlist?list=") != string::npos ||
           url.find("&list=") != string::npos;
}

void DownloadManager::downloadWithYtDlp() {
    string quality = "";
    if (numThreads == 1) quality = "bestvideo[height<=480]+bestaudio/best[height<=480]";
    else if (numThreads == 2) quality = "bestvideo[height<=720]+bestaudio/best[height<=720]";
    else if (numThreads == 3) quality = "bestvideo[height<=1080]+bestaudio/best[height<=1080]";
    else quality = "bestvideo+bestaudio/best";

    string audiofix = "--postprocessor-args \"-c:a aac\"";
    string command;

    if (isPlaylist()) {
        cout << "Playlist detected! Downloading all videos...\n";
        command = "yt-dlp " + audiofix + " -o \"" + outputPath + "/%(playlist_index)s-%(title)s.%(ext)s\" --merge-output-format mp4 -f \"" + quality + "\" " + url;
    } else {
        command = "yt-dlp " + audiofix + " -o \"" + outputPath + "/%(title)s.%(ext)s\" --merge-output-format mp4 -f \"" + quality + "\" " + url;
    }

    cout << "Using yt-dlp to download...\n";
    system(command.c_str());
    cout << "Download complete!\n";
}

void DownloadManager::start() {
    if (isYouTubeURL()) {
        downloadWithYtDlp();
        return;
    }

    size_t fileSize = getRemoteFileSize();
    if (fileSize == 0) {
        cout << "Could not get file size!\n";
        return;
    }

    cout << "File size: " << fileSize / (1024 * 1024) << " MB\n";
    cout << "Threads: " << numThreads << "\n";
    splitAndDispatch(fileSize);
    cout << "\nDownload complete!\n";
}
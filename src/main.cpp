#include <iostream>
#include "DownloadManager.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: downloader <url> <output_file> [threads]\n";
        return 1;
    }

    string url        = argv[1];
    string outputPath = argv[2];
    int threads       = (argc >= 4) ? stoi(argv[3]) : 4;

    DownloadManager manager(url, outputPath, threads);

    if (manager.hasResumeState()) {
        cout << "Resuming previous download...\n";
    }

    manager.start();

    return 0;
}
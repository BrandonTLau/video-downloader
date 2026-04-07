#pragma once
#include <string>
#include <vector>

using namespace std;

class ResumeState {
public:
    ResumeState(const string& filePath);
    void markChunkDone(int chunkIndex);
    bool isChunkDone(int chunkIndex);
    void save();
    void load();

private:
    string filePath;
    vector<bool> completedChunks;
};
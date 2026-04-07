#include "ResumeState.h"
#include <fstream>
#include <iostream>

using namespace std;

ResumeState::ResumeState(const string& filePath) {
    this->filePath = filePath + ".resume";
}

void ResumeState::markChunkDone(int chunkIndex) {
    if (chunkIndex < completedChunks.size())
        completedChunks[chunkIndex] = true;
    save();
}

bool ResumeState::isChunkDone(int chunkIndex) {
    if (chunkIndex < completedChunks.size())
        return completedChunks[chunkIndex];
    return false;
}

void ResumeState::save() {
    ofstream file(filePath);
    for (bool chunk : completedChunks)
        file << chunk << "\n";
}

void ResumeState::load() {
    ifstream file(filePath);
    if (!file.is_open()) return;
    bool val;
    while (file >> val)
        completedChunks.push_back(val);
}
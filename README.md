# Video Downloader

A multithreaded video downloader built in C++ with a Python GUI.
Supports direct file downloads and YouTube videos/playlists.

## Features
- Multithreaded direct file downloads (fast)
- YouTube single video and playlist support
- Quality selection (480p, 720p, 1080p, Best)
- Real-time progress bar with download speed
- Retry logic for failed chunks
- Resume support for interrupted downloads

## Requirements
- Windows 10/11
- MSYS2 UCRT64 with the following packages:
  - mingw-w64-ucrt-x86_64-gcc
  - mingw-w64-ucrt-x86_64-cmake
  - mingw-w64-ucrt-x86_64-curl
  - mingw-w64-ucrt-x86_64-ffmpeg
  - mingw-w64-ucrt-x86_64-python
  - yt-dlp

## Installation
1. Clone or download the repository
2. Open MSYS2 UCRT64 terminal
3. Navigate to the project folder:
cd /c/Users/<yourname>/video-downloader
4. Build the project:
cd build
cmake .. -G "Unix Makefiles"
make
cd ..

## Usage

### GUI
Run the GUI from the MSYS2 UCRT64 terminal:
python gui.py

- Paste any URL into the URL field
- Select an output folder
- For YouTube videos, select a quality
- Click Download

### Command Line
./build/downloader.exe <url> <output_path> <threads>

Examples:
Direct file download
./build/downloader.exe "http://example.com/video.mp4" "video.mp4" 4
YouTube video
./build/downloader.exe "https://www.youtube.com/watch?v=..." "output_folder" 4
YouTube playlist
./build/downloader.exe "https://www.youtube.com/playlist?list=..." "output_folder" 4

Thread count also controls YouTube quality:
- 1 = 480p
- 2 = 720p
- 3 = 1080p
- 4 = Best available

## Project Structure
video-downloader/
├── src/
│   ├── main.cpp
│   ├── Downloader.cpp
│   ├── DownloadManager.cpp
│   ├── ThreadPool.cpp
│   ├── ProgressTracker.cpp
│   └── ResumeState.cpp
├── include/
│   ├── Downloader.h
│   ├── DownloadManager.h
│   ├── ThreadPool.h
│   ├── ProgressTracker.h
│   └── ResumeState.h
├── build/
├── bundle/
│   ├── ffmpeg.exe
│   └── yt-dlp.exe
├── gui.py
├── CMakeLists.txt
└── README.md

## Libraries Used
- libcurl — HTTP requests and chunked downloads
- nlohmann/json — Resume state serialization
- yt-dlp — YouTube downloading
- ffmpeg — Video/audio merging
- tkinter — GUI

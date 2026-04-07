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

## Installation

### Step 1 — Install MSYS2
1. Go to [msys2.org](https://msys2.org) and download the installer
2. Run the installer and install to the default path (`C:\msys64`)
3. Open **MSYS2 UCRT64** from the Start Menu
4. Update everything:
pacman -Syu
   It may close the window — reopen MSYS2 UCRT64 and run:
pacman -Su

### Step 2 — Install Dependencies
In the **MSYS2 UCRT64** terminal run these one by one:
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-cmake
pacman -S mingw-w64-ucrt-x86_64-curl
pacman -S mingw-w64-ucrt-x86_64-ffmpeg
pacman -S mingw-w64-ucrt-x86_64-python
pacman -S mingw-w64-ucrt-x86_64-python-pip
pacman -S make
pacman -S git
pip install yt-dlp --break-system-packages

### Step 3 — Clone the Repository
git clone https://github.com/BrandonTLau/video-downloader.git
cd video-downloader

### Step 4 — Build the Project
cd build
cmake .. -G "Unix Makefiles"
make
cd ..

## How to Run

### GUI
1. Open **MSYS2 UCRT64** from the Start Menu
2. Navigate to the project folder:
cd /c/Users/<yourname>/video-downloader
3. Run:
python gui.py
4. Paste a URL into the URL field
5. Select an output folder using the Browse button
6. For YouTube videos select a quality (480p, 720p, 1080p, Best)
7. Click **Download**

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
├── gui.py
├── CMakeLists.txt
└── README.md

## Libraries Used
- libcurl — HTTP requests and chunked downloads
- yt-dlp — YouTube downloading
- ffmpeg — Video/audio merging
- tkinter — GUI

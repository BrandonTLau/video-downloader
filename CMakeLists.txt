cmake_minimum_required(VERSION 3.15)
project(VideoDownloader)

set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(WIN32_LEAN_AND_MEAN)

find_package(CURL REQUIRED)

include_directories(include)

add_executable(downloader
    src/main.cpp
    src/Downloader.cpp
    src/ThreadPool.cpp
    src/DownloadManager.cpp
    src/ProgressTracker.cpp
    src/ResumeState.cpp
)

target_link_libraries(downloader CURL::libcurl)
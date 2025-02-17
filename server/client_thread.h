#pragma once
#include <thread>
#include <fstream>
#include <mutex>
#include <string>
#include <cstring>
#include <memory>
#include <atomic>

#define BUFFER_SIZE (1008)

class ClientThread{
public:
    ClientThread(int clientfd);
    ~ClientThread();
    int getClientFd();
    void removeThread();

private:
    static std::mutex mtx;
    std::unique_ptr<std::thread> t;
    int clientfd_;
    void writeClientMessage();

    // 복사 금지 (std::thread는 복사할 수 없음)
    ClientThread(const ClientThread&) = delete;
    ClientThread& operator=(const ClientThread&) = delete;
};
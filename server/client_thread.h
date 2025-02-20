#pragma once
#include <thread>
#include <fstream>
#include <mutex>
#include <string>
#include <cstring>
#include <memory>
#include <atomic>

#define BUFFER_SIZE (1008)

class ClientThread
{
public:
    ClientThread(int clientfd);
    ~ClientThread();

    void removeThread();

private:
    void writeClientMessage();

    std::unique_ptr<std::thread> t;
    static std::mutex mtx;

    int clientfd_;
    std::atomic<bool> stop_;
};
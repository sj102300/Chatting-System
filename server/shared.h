#pragma once

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <mutex>
#include <thread>
#include <fstream>
#include <atomic>

#include "client_thread.h"
#define MAX_CLIENT (3)

class ClientThread;

class Shared
{
public:
    static Shared &Instance()
    {
        static Shared instance;
        return instance;
    }

    Shared(const Shared &) = delete;
    Shared &operator=(const Shared &) = delete;
    ~Shared();

    void queuePush(int clientfd);
    void arrangeClientThreads();
    void deleteClientThread();
    void removeClientFd(int clientfd);

private:
    Shared();

    std::atomic<bool> stop_;

    std::thread background_thread_;
    std::thread client_queue_management_thread_;

    std::mutex clientThreads_mtx;
    std::mutex deleteClientThreads_mtx;
    std::mutex waitingQueue_mtx;
    std::mutex file_mtx;

    std::unordered_set<int> deletedClientThreads;
    std::unordered_map<int, std::shared_ptr<ClientThread>> clientThreads;
    std::queue<int> waitingQueue;

    void clientQueueManagement();
};

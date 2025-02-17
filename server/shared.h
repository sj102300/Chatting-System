#pragma once
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>
#include <mutex>

#include "client_thread.h"
#define MAX_CLIENT (3)

class Shared{
private:
    static std::mutex clientThreads_mtx;
    static std::mutex waitingQueue_mtx;
    static std::mutex file_mtx;
    static std::mutex deletedClientThreads_mtx;
public:
    static std::unordered_set<int> deletedClientThreads;
    static std::unordered_map<int, std::shared_ptr<ClientThread>> clientThreads;
    static std::queue<int> waitingQueue;
    static std::ofstream file;

    static void queuePush(int clientfd);
    static void queuePop(bool hasMtx);
    static void removeClientFd(int clientfd);
    static void arrangeClientThreads();
    static void deleteClientThread();
    static void writeInputFile(std::string clientId, std::string buffer);
    static std::shared_ptr<ClientThread> getClientThread(int clientfd);
};
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <mutex>
#include <memory>
#include <unistd.h>

#include "shared.h"

Shared::Shared() : stop_(false)
{
    background_thread_ = std::thread(&Shared::arrangeClientThreads, this);
    client_queue_management_thread_ = std::thread(&Shared::clientQueueManagement, this);
}

Shared::~Shared()
{
    stop_ = true;
    if (background_thread_.joinable())
        background_thread_.join();
    if (client_queue_management_thread_.joinable())
        client_queue_management_thread_.join();

    std::cout << "Shared 소멸" << std::endl;
}

void Shared::queuePush(int clientfd)
{
    waitingQueue.push(clientfd);
}

void Shared::arrangeClientThreads()
{
    while (!stop_)
    {
        deleteClientThread();
        std::cout << "arrange중" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    std::cout << "arrange thread를 종료합니다." << std::endl;
}

void Shared::deleteClientThread()
{
    std::lock_guard<std::mutex> lock_client_threads(deleteClientThreads_mtx);
    if (deletedClientThreads.size() <= 0)
        return;

    for (auto it = deletedClientThreads.begin(); it != deletedClientThreads.end(); ++it)
    {
        std::lock_guard<std::mutex> lock_client_threads(clientThreads_mtx);
        clientThreads[*it]->removeThread();
        clientThreads.erase(*it);
    }
    deletedClientThreads.clear();
}

void Shared::clientQueueManagement()
{
    while (!stop_)
    {
        if (clientThreads.size() >= MAX_CLIENT)
            continue;

        if (waitingQueue.empty())
            continue;

        const int target = waitingQueue.front();
        waitingQueue.pop();
        clientThreads.insert({target, std::make_shared<ClientThread>(target)});
        std::cout << "clientThreads size : " << clientThreads.size() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    if (stop_)
    {
        std::lock_guard<std::mutex> lock_client_threads(clientThreads_mtx);
        for (auto &pair : clientThreads)
        {
            pair.second->removeThread();
        }
        clientThreads.clear();
    }
    std::cout << "clientQueueManagement thread를 종료합니다." << std::endl;
}

void Shared::removeClientFd(int clientfd)
{
    std::lock_guard<std::mutex> lock_client_threads(deleteClientThreads_mtx);
    deletedClientThreads.insert(clientfd);
}
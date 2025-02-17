#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <mutex>
#include <memory>
#include <unistd.h>
#include "shared.h"
#define MAX_CLIENT (3)

std::mutex Shared::clientThreads_mtx;
std::mutex Shared::waitingQueue_mtx;
std::mutex Shared::file_mtx;
std::mutex Shared::deletedClientThreads_mtx;

std::unordered_set<int> Shared::deletedClientThreads;
std::unordered_map<int, std::shared_ptr<ClientThread>> Shared::clientThreads;
std::queue<int> Shared::waitingQueue;
std::ofstream Shared::file("input.txt", std::ios::app);

void Shared::queuePush(int clientfd) {
    {
        std::lock_guard<std::mutex> lock(waitingQueue_mtx);
        waitingQueue.push(clientfd);
    }
    queuePop(false);
}

void Shared::queuePop(bool hasMtx){
    if(!hasMtx)
        std::lock_guard<std::mutex> lock(clientThreads_mtx);
    
    if(clientThreads.size() >= MAX_CLIENT)
        return;
    
    int target;
    {
        std::lock_guard<std::mutex> lock(waitingQueue_mtx);
        if(waitingQueue.empty())
            return;
        target = waitingQueue.front();
        waitingQueue.pop();
    }
    clientThreads.insert({target, std::make_shared<ClientThread>(target)});
}

void Shared::removeClientFd(int clientfd){
    std::lock_guard<std::mutex> lock(deletedClientThreads_mtx);
    deletedClientThreads.insert(clientfd);
}

void Shared::arrangeClientThreads(){
    while (true){
        deleteClientThread();
        std::cout<<"arrange중"<<std::endl;
        sleep(1);
    }
}

void Shared::deleteClientThread(){
    std::lock_guard<std::mutex> lock2(deletedClientThreads_mtx);
    if(deletedClientThreads.size() <= 0)
        return;
    std::lock_guard<std::mutex> lock(clientThreads_mtx);
    for(auto it = deletedClientThreads.begin(); it!=deletedClientThreads.end();it++){
        clientThreads[*it]->removeThread();
        clientThreads.erase(*it);
        queuePop(true);
    }
    deletedClientThreads.clear();
}

void Shared::writeInputFile(std::string clientId, std::string buffer){
    {
        std::lock_guard<std::mutex> lock(file_mtx);
        Shared::file<<"클라이언트" << clientId <<": " <<buffer<<std::endl;
    }
}

std::shared_ptr<ClientThread> Shared::getClientThread(int clientfd){
    std::lock_guard<std::mutex> lock(clientThreads_mtx);
    auto it = clientThreads.find(clientfd);
    if (it == clientThreads.end()) return nullptr;
    return it->second; 
}
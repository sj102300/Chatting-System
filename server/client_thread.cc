#include <thread>
#include <fstream>
#include <mutex>
#include <string>
#include <cstring>
#include <memory>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "shared.h"

ClientThread::ClientThread(int clientfd) : clientfd_(clientfd), stop_(false)
{
    t = std::make_unique<std::thread>(&ClientThread::writeClientMessage, this);
}

ClientThread::~ClientThread()
{
    stop_ = true;
    if (t->joinable())
    {
        t->join();
    }
    std::cout << "client thread 객체 소멸" << std::endl;
}

void ClientThread::removeThread()
{
    stop_ = true;
    if (t->joinable())
    {
        t->join();
    }
    std::cout << "thread join() 끝" << std::endl;
}

#define BUFFER_SIZE (1008)

void ClientThread::writeClientMessage()
{
    std::cout << "writeClientMEssage호출" << std::endl;
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(clientfd_, buffer, sizeof(buffer), 0);

    // clientId설정
    std::string clientId;
    if (bytes_received > 0)
    {
        clientId = buffer;
    }
    else
    {
        clientId = std::to_string(clientfd_);
    }

    // 파일에 쓰기
    while (!stop_)
    {
        int bytes_received = recv(clientfd_, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0)
        {
            break;
        }
        std::cout << buffer << std::endl;
        std::string buffertostr(buffer);
        if (buffertostr == "exit")
        {
            break;
        }
        memset(buffer, 0, sizeof(buffer));
    }

    Shared &shared_ = Shared::Instance();
    shared_.removeClientFd(clientfd_);
    std::cout << "thread 작업 종료" << std::endl;
}

#include <thread>
#include <fstream>
#include <mutex>
#include <string>
#include <cstring>
#include <memory>

#include <sys/socket.h>  // recv() 함수 정의
#include <unistd.h>      // close() 함수 등

#include "shared.h"

#include <iostream>

ClientThread::ClientThread(int clientfd) : clientfd_(clientfd){
    t = std::make_unique<std::thread>(&ClientThread::writeClientMessage, this);
}

int ClientThread::getClientFd(){
    return clientfd_;
}

ClientThread::~ClientThread(){
    if(t->joinable()){
        t->join();
    }
    std::cout<<"client thread 객체 소멸"<<std::endl;
}

void ClientThread::removeThread(){
    if(t->joinable()){
        t->join();
    }
    std::cout<<"thread join() 끝"<<std::endl;
}

#define BUFFER_SIZE (1008)
void ClientThread::writeClientMessage(){

    std::shared_ptr<ClientThread> self = Shared::getClientThread(clientfd_);
    
    std::cout<<"writeClientMEssage호출"<<std::endl;
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(clientfd_, buffer, sizeof(buffer), 0);
    
    //clientId설정
    std::string clientId;
    if(bytes_received > 0){
        clientId = buffer;
    }
    else{
        clientId = std::to_string(clientfd_);
    }
    
    //파일에 쓰기
    while(true){
        int bytes_received = recv(clientfd_, buffer, sizeof(buffer), 0);
        if(bytes_received <= 0){
            break;
        } 
        std::cout<<buffer<<std::endl;
        std::string buffertostr(buffer);
        if(buffertostr == "exit"){
            break;
        }
        Shared::writeInputFile(clientId, buffertostr);
        memset(buffer, 0, sizeof(buffer));
    }
    Shared::removeClientFd(clientfd_);
    std::cout<<"thread 작업 종료"<<std::endl;
}

#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define LOCALHOST "127.0.0.1"

void send_message(int client_socket){
    std::string msg;

    while(true){
        std::cout<<"보낼 메시지: ";
        std::getline(std::cin, msg);

        send(client_socket, msg.c_str(), msg.length(), 0);

        if(msg == "exit"){
            break;
        }
    }
}

int main(){
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1){
        std::cerr<<"소켓 생성 실패"<<std::endl;
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(LOCALHOST);

    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        std::cerr<<"서버 연결 실패"<<std::endl;
        return -1;
    }

    std::cout<<"서버에 연결됨"<<std::endl;

    send_message(client_socket);
    close(client_socket);

}
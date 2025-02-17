#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080
#define LOCALHOST "127.0.0.1"

void send_message(int clientsocket){
    std::string msg;

    std::cout<<"Id : ";
    std::getline(std::cin, msg);
    
    send(clientsocket, msg.c_str(), msg.length(), 0);

    while(true){
        std::cout<<"보낼 메시지: ";
        std::getline(std::cin, msg);

        send(clientsocket, msg.c_str(), msg.length(), 0);

        if(msg == "exit"){
            return;
        }
    }
}

int main(){
    int clientsocket;

    if((clientsocket = socket(AF_INET, SOCK_STREAM, 0) )< 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr, clinet_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
    server_addr.sin_port = htons(PORT);

    if(connect(clientsocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    std::cout<<"서버에 연결됨"<<std::endl;

    send_message(clientsocket);
    close(clientsocket);

}

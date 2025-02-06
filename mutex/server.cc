
#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <netinet/tcp.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

std::string filePath = "input.txt";
std::mutex mtx;
std::vector<std::thread> client_threads;

void handle_client(int client_socket)
{
    char buffer[BUFFER_SIZE];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0)
        {
            std::cout << "클라이언트" <<  client_socket << " 종료" << std::endl;
            break;
        }

        {
            std::lock_guard<std::mutex> lock(mtx);
            std::ofstream file(filePath, std::ios::app);
            file << "클라이언트" << client_socket << ": " << buffer << std::endl;
        }
    }
    close(client_socket);
}

int main()
{
    int serverfd, client_socket;

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
     if (serverfd == -1)
    {
        std::cerr << "소켓 생성 실패" << std::endl;
        return -1;
    }

    int flag = 1;
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind fail");
        exit(EXIT_FAILURE);
        std::cerr << "바인딩 실패" << std::endl;
        return -1;
    }

    if (listen(serverfd, MAX_CLIENTS) == -1)
    {
        std::cerr << "listening fail" << std::endl;
        return -1;
    }

    std::cout << "서버 실행중 ... " << std::endl;

    while (true)
    {
        client_socket = accept(serverfd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1)
        {
            std::cerr << "클라이언트 연결 실패" << std::endl;
            continue;
        }
        std::cout << "클라이언트 연결됨!" << std::endl;

        client_threads.emplace_back(std::thread(handle_client, client_socket));
    }

    close(serverfd);
    return 0;
}
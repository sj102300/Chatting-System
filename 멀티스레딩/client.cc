#include <iostream>
#include <cstring>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <sstream>

#include "data_struct.h"

#define SERVER_IP "127.0.0.1" // localhost ip (loop back)
#define PORT 8080
#define READ_SIZE 1008

int main()
{
    int sock;
    struct sockaddr_in server_address;
    char buffer[READ_SIZE] = {0};

    // 1️⃣ 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        return -1;
    }

    // 2️⃣ 서버 주소 설정
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0)
    {
        perror("Invalid address / Address not supported");
        return -1;
    }

    // 3️⃣ 서버에 연결
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Connection failed");
        return -1;
    }

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::cout << "✉️ 서버에 연결됨!";
    Body *body = (Body *)buffer;
    std::string filePath = "input" + std::to_string(time) + ".txt";

    std::ofstream file(filePath, std::ios::out);

    if (!file)
    {
        perror("파일 열기 불가");
    }

    while (true)
    {
        ssize_t recvbytes = recv(sock, buffer, READ_SIZE, 0);
        if (recvbytes > 0)
        {
            for (int i = 0; i < body->data_len; ++i)
            {
                file << body->data[i];
            }
            if (body->flag == 1)
            {
                break;
            }
            memset(buffer, 0, READ_SIZE);
        };
    }

    // 4️⃣ 종료
    close(sock);
    return 0;
}

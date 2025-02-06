#include <iostream>
#include <cstring>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include <fstream>
#include <sstream>

#include "data_struct.h"

#define PORT 8080
#define SEND_SIZE (1008)

int client_sockets[3] = {-1, -1, -1};

int get_client_sockets_idx()
{
    for (int i = 0; i < 3; ++i)
    {
        if (client_sockets[i] == -1)
            return i;
    }
    return -1;
}

void handle_client(int client_socket, int client_idx)
{

    std::ifstream file("/root/LigNex1/Socket/input.txt");
    if (!file)
    {
        std::cerr << "파일열기 불가\n";
        return;
    }

    Body body = {0};
    int flag = 0;
    unsigned int data_len = 0;
    while (!file.eof())
    {
        if (!file.read(body.data, DATA_SIZE))
        {
            body.flag = 1;
        }

        if (file.gcount() > 0)
        {
            body.data_len = static_cast<unsigned int>(file.gcount());
        }
        ssize_t sendbyte = send(client_socket, &body, SEND_SIZE, 0);
        sleep(2);
        if (sendbyte < 0)
        {
            perror("send failed");
        }
        body = {0};
    }

    client_sockets[client_idx] = -1;
    return;
}

int main()
{
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    std::vector<std::thread> clients;

    // 1️⃣ 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int flag = 1;
    setsockopt(server_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));

    // 2️⃣ 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 3️⃣ 소켓 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 4️⃣ 연결 대기
    if (listen(server_fd, 5) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "🔥 Echo 서버가 포트 " << PORT << "에서 실행 중..." << std::endl;

    // 5️⃣ 클라이언트 요청 수락
    int client_idx;
    while (true)
    {
        if ((client_idx = get_client_sockets_idx()) >= 0)
        {
            if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("Accept failed");
            }
            std::cout << "✅ 클라이언트 연결됨!" << std::endl;
            clients.emplace_back(handle_client, client_socket, client_idx);
            client_sockets[client_idx] = client_socket;
        }
    }

    std::cout << "소켓 닫기" << std::endl;
    close(server_fd);
    return 0;
}

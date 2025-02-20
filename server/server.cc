#include <iostream>
#include <queue>
#include <vector>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <algorithm>
#include <thread>

#include "shared.h"
#include "server.h"

#define PORT (8080)

void runServer()
{
    int serverfd, clientfd;
    struct sockaddr_in server_addr, client_addr;

    if (setServer(serverfd, server_addr))
    {
        std::cout << "서버 실행중..." << std::endl;
    }

    fd_set readfds;
    int maxfd = serverfd;

    // Shared controlThrhead;
    Shared &controlThread = Shared::Instance();

    while (true)
    {
        FD_ZERO(&readfds);
        FD_SET(serverfd, &readfds); // 서버 소켓 감시 설정
        socklen_t addrlen = sizeof(client_addr);

        // nfds: 감시할 파일 디스크립터 중 가장 큰 값 + 1
        if (select(maxfd + 1, &readfds, 0, 0, 0) < 0)
        { // 변화된 fd값만 남기기
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(serverfd, &readfds))
        {
            if ((clientfd = accept(serverfd, (struct sockaddr *)&client_addr, &addrlen)) < 0)
            {
                perror("client accept failed");
                exit(EXIT_FAILURE);
            }
            controlThread.queuePush(clientfd);
            maxfd = std::max(maxfd, clientfd);
        }
    }

    return;
}

bool setServer(int &serverfd, struct sockaddr_in &server_addr)
{

    if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int flag = 1;
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    if (bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverfd, MAX_CLIENT) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    return true;
}
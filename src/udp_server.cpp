#include "udp_server.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <atomic>

namespace udp_server
{

    UDPServer::UDPServer() : sockfd(-1), isRunning(false) {}

    UDPServer::~UDPServer()
    {
        stop();
    }

    bool UDPServer::initialize(int port)
    {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0)
        {
            perror("ソケット作成失敗");
            return false;
        }

        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            perror("バインド失敗");
            close(sockfd);
            sockfd = -1;
            return false;
        }

        std::cout << "UDPサーバー起動 (ポート: " << port << ")" << std::endl;
        return true;
    }

    bool UDPServer::startListening(DataReceivedCallback callback)
    {
        if (sockfd < 0)
        {
            std::cerr << "Error: Server not initialized." << std::endl;
            return false;
        }
        dataCallback = callback;
        isRunning = true;
        std::thread serverThread(&UDPServer::serverLoop, this);
        serverThread.detach(); // デタッチしてバックグラウンドで実行
        return true;
    }

    void UDPServer::stop()
    {
        isRunning = false;
        if (sockfd >= 0)
        {
            close(sockfd);
            sockfd = -1;
        }
        std::cout << "UDP server stopped." << std::endl;
    }

    void UDPServer::serverLoop()
    {
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        char buffer[1024];

        while (isRunning)
        {
            int recvLen = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&clientAddr, &addrLen);
            if (recvLen < 0)
            {
                perror("受信エラー");
                continue;
            }

            buffer[recvLen] = '\0';
            std::string receivedData(buffer);
            if (dataCallback)
            {
                dataCallback(receivedData);
            }
        }
    }

} // namespace udp_server

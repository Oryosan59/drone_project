#include "udp_client.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

namespace udp_client
{

    UDPClient::UDPClient() : send_socket(-1) {}

    UDPClient::~UDPClient()
    {
        close();
    }

    bool UDPClient::initialize(const char *server_ip, int server_port, int client_port)
    {
        send_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (send_socket < 0)
        {
            perror("Error creating send socket");
            return false;
        }

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(server_ip);
        server_addr.sin_port = htons(server_port);

        memset(&client_addr, 0, sizeof(client_addr));
        client_addr.sin_family = AF_INET;
        client_addr.sin_addr.s_addr = INADDR_ANY;
        client_addr.sin_port = htons(client_port);

        if (bind(send_socket, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
        {
            perror("Error binding client socket");
            close(send_socket);
            send_socket = -1;
            return false;
        }

        return true;
    }

    bool UDPClient::sendData(const char *data)
    {
        if (send_socket < 0)
        {
            std::cerr << "Error: Client not initialized." << std::endl;
            return false;
        }
        ssize_t sent_len = sendto(send_socket, data, strlen(data), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent_len < 0)
        {
            perror("Error sending data");
            return false;
        }
        return true;
    }

    void UDPClient::close()
    {
        if (send_socket >= 0)
        {
            ::close(send_socket);
            send_socket = -1;
        }
    }

} // namespace udp_client

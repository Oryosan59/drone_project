#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <string>

namespace udp_client
{

    class UDPClient
    {
    public:
        UDPClient();
        ~UDPClient();

        bool initialize(const char *server_ip, int server_port, int client_port);
        bool sendData(const char *data);
        void close();

    private:
        int send_socket;
        struct sockaddr_in server_addr;
        struct sockaddr_in client_addr;
    };

} // namespace udp_client

#endif

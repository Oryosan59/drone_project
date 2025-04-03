#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <string>
#include <functional>

namespace udp_server
{

    class UDPServer
    {
    public:
        using DataReceivedCallback = std::function<void(const std::string &)>;

        UDPServer();
        ~UDPServer();

        bool initialize(int port);
        bool startListening(DataReceivedCallback callback);
        void stop();

    private:
        int sockfd;
        bool isRunning;
        DataReceivedCallback dataCallback;
        void serverLoop();
    };

} // namespace udp_server

#endif

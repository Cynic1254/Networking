#ifndef SOCKETS_HPP
#define SOCKETS_HPP
#include <winsock2.h>

#include "Address.hpp"

namespace CynNet
{
    class Socket
    {
    public:
        Socket(const IP& ip, ConnectionType protocol);
        virtual ~Socket();

    protected:
        Socket(SOCKET socket, IP ip, ConnectionType protocol);

        IP ip;
        ConnectionType protocol;
        SOCKET socketObject = INVALID_SOCKET;
    };

    class UDPSocket : private Socket
    {
    public:
        explicit UDPSocket(const IP& ip);

        [[nodiscard]] int Send(const std::vector<char>& data) const;
        [[nodiscard]] std::vector<char> Receive() const;
    };

    class TCPSocket : private Socket
    {
    public:
        explicit TCPSocket(const IP& ip);
        [[nodiscard]] int Send(const std::vector<char>& data) const;
        [[nodiscard]] std::vector<char> Receive() const;

    private:
        friend class TCPServer;
        TCPSocket(SOCKET socket, const IP& ip);
    };

    class TCPServer : private Socket
    {
    public:
        explicit TCPServer(const IP& ip, int maxConnections = SOMAXCONN);

        [[nodiscard]] std::vector<TCPSocket> Accept() const;
    };
} // namespace CynNet

#endif  // SOCKETS_HPP
 

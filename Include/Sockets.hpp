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
        ~Socket();

        int Send(const char* data, int size) const;
        [[nodiscard]] std::vector<char> Receive(int size) const;

    private:
        IP ip;
        ConnectionType protocol;
        SOCKET socketObject = INVALID_SOCKET;
    };
} // namespace CynNet

#endif  // SOCKETS_HPP
 

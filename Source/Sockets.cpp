#include "Sockets.hpp"

#include <iostream>

#include "wsadata.hpp"

namespace CynNet
{
    Socket::Socket(const IP& ip, ConnectionType protocol) :
        ip(ip), protocol(protocol)
    {
        Init();

        socketObject = socket(static_cast<int>(ip.Version()), static_cast<int>(protocol),
                              protocol == ConnectionType::TCP ? IPPROTO_TCP : IPPROTO_UDP);
        if (socketObject == INVALID_SOCKET)
        {
            std::cerr << "Socket creation failed! " << WSAGetLastError() << std::endl;
            return;
        }

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.S_un.S_addr = INADDR_ANY;
        address.sin_port = htons(static_cast<int>(ip.Port()));

        if (bind(socketObject, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            std::cerr << "Socket bind failed! " << WSAGetLastError() << std::endl;
        }

        if (protocol == ConnectionType::TCP)
        {
            if (connect(socketObject, static_cast<const sockaddr*>(ip), sizeof(sockaddr_storage)) == SOCKET_ERROR)
                std::cerr << "Socket connection failed! " << WSAGetLastError() << std::endl;
        }
    }

    Socket::~Socket()
    {
        if (protocol == ConnectionType::TCP)
            shutdown(socketObject, SD_BOTH);

        closesocket(socketObject);
        socketObject = INVALID_SOCKET;
    }

    int Socket::Send(const char* data, const int size) const
    {
        int result = 0;

        if (protocol == ConnectionType::TCP)
            result = send(socketObject, data, size, 0);
        else
            result = sendto(socketObject, data, size, 0, ip.SockAddr(), ip.GetSockAddrSize());

        if (result == SOCKET_ERROR)
        {
            std::cerr << "Socket send failed! " << WSAGetLastError() << std::endl;
            return 0;
        }
        return result;
    }

    std::vector<char> Socket::Receive(const int size) const
    {
        std::vector<char> result;
        result.resize(size);

        int output = 0;

        if (protocol == ConnectionType::TCP)
            output = recv(socketObject, result.data(), size, 0);
        else
            output = recvfrom(socketObject, result.data(), size, 0, nullptr, nullptr);

        if (output == SOCKET_ERROR)
        {
            std::cerr << "Socket receive failed! " << WSAGetLastError() << std::endl;
            result.clear();
        }
        return result;
    }
} // CynNet

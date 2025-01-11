#include "Sockets.hpp"

#include <iostream>
#include <utility>

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
        address.sin_family = static_cast<short>(ip.Version());
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

    Socket::Socket(const SOCKET socket, IP ip, const ConnectionType protocol) :
        ip(std::move(ip)), protocol(protocol), socketObject(socket)
    {
    }

    UDPSocket::UDPSocket(const IP& ip) :
        Socket(ip, ConnectionType::UDP)
    {
    }

    int UDPSocket::Send(const std::vector<char>& data) const
    {
        int result = 0;

        result = sendto(socketObject, data.data(), static_cast<int>(data.size()), 0, ip.SockAddr(),
                        ip.GetSockAddrSize());

        if (result == SOCKET_ERROR)
        {
            std::cerr << "Socket send failed! " << WSAGetLastError() << std::endl;
            return 0;
        }

        return result;
    }

    std::vector<char> UDPSocket::Receive(const u_long sizeLimit) const
    {
        std::vector<char> result;

        const u_long bytes = GetPendingBytes();
        if (bytes == SOCKET_ERROR)
        {
            std::cerr << "Socket receive failed! " << WSAGetLastError() << std::endl;
            return result;
        }
        if (bytes == 0)
            return result;

        if (sizeLimit > 0)
            result.resize(bytes < sizeLimit ? bytes : sizeLimit);
        else
            result.resize(bytes);

        int received = recvfrom(socketObject, result.data(), static_cast<int>(result.size()), 0, nullptr, nullptr);

        if (received == SOCKET_ERROR)
        {
            std::cerr << "Socket receive failed! " << WSAGetLastError() << std::endl;
            result.clear();
            return result;
        }

        result.resize(received);
        return result;
    }

    u_long UDPSocket::GetPendingBytes() const
    {
        u_long bytes = 0;
        if (ioctlsocket(socketObject, FIONREAD, &bytes) == SOCKET_ERROR)
        {
            std::cerr << "Socket receive failed! " << WSAGetLastError() << std::endl;
        }

        return bytes;
    }

    TCPSocket::TCPSocket(const IP& ip) :
        Socket(ip, ConnectionType::TCP)
    {
    }

    int TCPSocket::Send(const std::vector<char>& data) const
    {
        int result = 0;

        result = send(socketObject, data.data(), static_cast<int>(data.size()), 0);

        if (result == SOCKET_ERROR)
        {
            std::cerr << "Socket send failed! " << WSAGetLastError() << std::endl;
            return 0;
        }

        return result;
    }

    std::vector<char> TCPSocket::Receive(const u_long sizeLimit) const
    {
        std::vector<char> result;

        const u_long bytes = GetPendingBytes();
        if (bytes == SOCKET_ERROR)
        {
            std::cerr << "Socket receive failed! " << WSAGetLastError() << std::endl;
            return result;
        }
        if (bytes == 0)
            return result;

        if (sizeLimit > 0)
            result.resize(bytes < sizeLimit ? bytes : sizeLimit);
        else
            result.resize(bytes);

        int received = recv(socketObject, result.data(), static_cast<int>(result.size()), 0);

        if (received == SOCKET_ERROR)
        {
            std::cerr << "Socket receive failed! " << WSAGetLastError() << std::endl;
            result.clear();
            return result;
        }

        result.resize(received);
        return result;
    }

    u_long TCPSocket::GetPendingBytes() const
    {
        u_long bytes = 0;
        if (ioctlsocket(socketObject, FIONREAD, &bytes) == SOCKET_ERROR)
        {
            std::cerr << "Socket receive failed! " << WSAGetLastError() << std::endl;
        }

        return bytes;
    }

    TCPSocket::TCPSocket(const SOCKET socket, const IP& ip) :
        Socket(socket, ip, ConnectionType::TCP)
    {
    }

    TCPServer::TCPServer(const IP& ip, int maxConnections) :
        Socket(ip, ConnectionType::TCP)
    {
        if (listen(socketObject, maxConnections) == SOCKET_ERROR)
        {
            std::cerr << "Socket listen failed! " << WSAGetLastError() << std::endl;
        }
    }

    std::vector<TCPSocket> TCPServer::Accept() const
    {
        std::vector<TCPSocket> result;
        fd_set fds{};
        FD_ZERO(&fds);
        FD_SET(socketObject, &fds);

        do
        {
            sockaddr_storage address{};
            socklen_t address_length = sizeof(address);

            SOCKET newSocket = accept(socketObject, reinterpret_cast<sockaddr*>(&address), &address_length);

            if (newSocket != INVALID_SOCKET)
            {
                result.emplace_back(TCPSocket{newSocket, IP{reinterpret_cast<sockaddr*>(&address)}});
            }
        }
        while (GetPendingConnections() > 0);

        return result;
    }

    int TCPServer::GetPendingConnections() const
    {
        fd_set fds{};
        FD_ZERO(&fds);
        FD_SET(socketObject, &fds);
        return select(0, &fds, nullptr, nullptr, nullptr);
    }
} // CynNet

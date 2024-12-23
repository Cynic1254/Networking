#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <memory>
#include <string>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace CynNet
{
    enum class IpVersion { IPv4 = AF_INET, IPv6 = AF_INET6, Unspecified = -1 };

    enum class ConnectionType { TCP = SOCK_STREAM, UDP = SOCK_DGRAM, Unspecified = 0 };

    /// @brief Represents a singular numerical ip address, either IPV4 or IPV6 which can be checked using Version
    struct IP
    {
        explicit IP(sockaddr* ip);
        explicit IP(const sockaddr_in& ipv4);
        explicit IP(const sockaddr_in6& ipv6);
        IP(const std::string& ip, int port, IpVersion version);

        [[nodiscard]] IpVersion Version() const { return version; }
        [[nodiscard]] int Port() const { return port; }
        [[nodiscard]] std::string ToString() const { return ipString; }

        explicit operator sockaddr*() { return reinterpret_cast<sockaddr*>(&ip); }

    private:
        sockaddr_storage ip{};
        IpVersion version;
        std::string ipString;
        int port = 0;
    };

    /// @brief Represents all IP's that are linked to a specific hostname
    class Address
    {
    public:
        Address(const std::string& hostname, int port, ConnectionType connection_type = ConnectionType::Unspecified);

        [[nodiscard]] const std::string& GetAddressName() const { return ipString; }
        [[nodiscard]] const std::vector<std::pair<IP, ConnectionType>>& GetIP4Addresses() const { return ip4s; }
        [[nodiscard]] const std::vector<std::pair<IP, ConnectionType>>& GetIP6Addresses() const { return ip6s; }

    protected:
    private:
        void Parse(addrinfo* info);

        std::string ipString;
        std::vector<std::pair<IP, ConnectionType>> ip4s;
        std::vector<std::pair<IP, ConnectionType>> ip6s;
    };
} // namespace CynNet

#endif // ADDRESS_HPP

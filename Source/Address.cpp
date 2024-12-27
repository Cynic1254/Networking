#include "Address.hpp"

#include <iostream>
#include <string>

#include "wsadata.hpp"

CynNet::IP::IP(sockaddr* ip)
{
    if (ip == nullptr)
    {
        version = IpVersion::Unspecified;
        return;
    }

    version = static_cast<IpVersion>(ip->sa_family);

    switch (version)
    {
    case IpVersion::IPv4:
    {
        const auto ip4 = reinterpret_cast<sockaddr_in*>(ip);
        memcpy(&this->ip, ip4, sizeof(sockaddr_in));
        port = ntohs(ip4->sin_port);

        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ip4->sin_addr, ip_str, INET_ADDRSTRLEN);
        ipString = ip_str;

        break;
    }
    case IpVersion::IPv6:
    {
        const auto ip6 = reinterpret_cast<sockaddr_in6*>(ip);
        memcpy(&this->ip, ip6, sizeof(sockaddr_in6));
        port = ntohs(ip6->sin6_port);

        char ip_str[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &ip6->sin6_addr, ip_str, INET6_ADDRSTRLEN);
        ipString = ip_str;

        break;
    }
    default:
        return;
    }
}

CynNet::IP::IP(const sockaddr_in& ipv4) :
    version(IpVersion::IPv4), port(ntohs(ipv4.sin_port))
{
    memcpy(&ip, &ipv4, sizeof(sockaddr_in));

    char ip_str[INET_ADDRSTRLEN] = {};
    inet_ntop(AF_INET, &ipv4.sin_addr, ip_str, sizeof(ip_str));
    ipString = ip_str;
}

CynNet::IP::IP(const sockaddr_in6& ipv6) :
    version(IpVersion::IPv6), port(ntohs(ipv6.sin6_port))
{
    memcpy(&ip, &ipv6, sizeof(sockaddr_in6));

    char ip_str[INET6_ADDRSTRLEN] = {};
    inet_ntop(AF_INET6, &ipv6.sin6_addr, ip_str, sizeof(ip_str));
    ipString = ip_str;
}

CynNet::IP::IP(const std::string& ip, const int port, const IpVersion version) :
    version(version), ipString(ip), port(port)
{
    int err = 0;

    switch (version)
    {
    case IpVersion::IPv4:
    {
        sockaddr_in ipv4{};
        ipv4.sin_family = AF_INET;
        ipv4.sin_port = port;
        err = inet_pton(AF_INET, ip.c_str(), &ipv4.sin_addr);
        memcpy(&this->ip, &ipv4, sizeof(ipv4));
        break;
    }
    case IpVersion::IPv6:
    {
        sockaddr_in6 ipv6{};
        ipv6.sin6_family = AF_INET6;
        ipv6.sin6_port = port;
        err = inet_pton(AF_INET6, ip.c_str(), &ipv6.sin6_addr);
        memcpy(&this->ip, &ipv6, sizeof(ipv6));
        break;
    }
    default:
        this->ip = {};
    }

    if (err != 1)
    {
        std::cerr << "Invalid address provided: " << ip << " does not map to "
            << (version == IpVersion::IPv4 ? "Ipv4" : "Ipv6") << std::endl;
    }
}

int CynNet::IP::GetSockAddrSize() const
{
    return version == IpVersion::IPv4 ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
}

CynNet::Address::Address(const std::string& hostname, const int port, ConnectionType connection_type) :
    ipString(hostname)
{
    Init();

    addrinfo hints{}, *info;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = static_cast<int>(connection_type);

    switch (connection_type)
    {
    case ConnectionType::TCP:
        hints.ai_protocol = IPPROTO_TCP;
    case ConnectionType::UDP:
        hints.ai_protocol = IPPROTO_UDP;
    default:
        hints.ai_protocol = 0;
    }

    int result = getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints, &info);
    if (result != 0)
    {
        // TODO: Handle errors
        switch (result)
        {
        case WSATRY_AGAIN:
            //TODO: implement retry system
            break;
        default:
            std::cerr << "getaddrinfo() failed: " << gai_strerror(result) << std::endl;
            break;
        }
    }

    // extract data...
    Parse(info);

    freeaddrinfo(info);
}

void CynNet::Address::Parse(addrinfo* info)
{
    for (auto iter = info; iter != nullptr; iter = iter->ai_next)
    {
        switch (iter->ai_family)
        {
        case AF_INET:
            ip4s.emplace_back(IP{iter->ai_addr}, static_cast<ConnectionType>(iter->ai_socktype));
            break;
        case AF_INET6:
            ip6s.emplace_back(IP{iter->ai_addr}, static_cast<ConnectionType>(iter->ai_socktype));
            break;
        default:
            break;
        }
    }
}

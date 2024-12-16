#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <winsock2.h>
#include <ws2tcpip.h>

namespace CynNet {

enum class AddrsFamily { Unspecified = AF_UNSPEC, IPV4 = AF_INET, IPV6 = AF_INET6, Other = AF_MAX };
enum class ConnectionType { TCP = SOCK_STREAM, UDP = SOCK_DGRAM };

class Address {
    public:
        Address(AddrsFamily ip_type, ConnectionType connection_type);

    protected:
    private:
        addrinfo info;
};
} // namespace CynNet

#endif // ADDRESS_HPP

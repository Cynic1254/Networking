#include <iostream>

#include "Address.hpp"
#include "Sockets.hpp"


int main(int argc, const char* argv[])
{
    const auto address = CynNet::Address{"localhost", 3000, CynNet::ConnectionType::UDP};
    const auto socket = CynNet::UDPSocket{address.GetIP4Addresses()[0].first};

    char data[] = "Hello World!, this is extra data to test the limits!";
    const std::vector<char> bytes{data, data + sizeof(data)};

    const int i = socket.Send(bytes);
    const auto recvData = socket.Receive(TODO);

    std::cout << recvData.data() << std::endl;

    if (recvData.size() == i)
    {
        return 0;
    }

    return 1;
}

#include <iostream>

#include "Address.hpp"
#include "Sockets.hpp"


int main(int argc, const char* argv[])
{
    const auto address = CynNet::Address{"localhost", 3000, CynNet::ConnectionType::UDP};
    const auto socket = CynNet::UDPSocket{address.GetIP4Addresses()[0].first};

    char data[] = "Hello World!, this is extra data to test the limits!";
    std::vector<char> bytes{data, data + sizeof(data)};

    int i = socket.Send(bytes);

    std::cout << socket.Receive().data() << std::endl;

    return 0;
}

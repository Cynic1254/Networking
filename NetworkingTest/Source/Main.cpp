#include <iostream>

#include "Address.hpp"
#include "Sockets.hpp"


int main(int argc, const char* argv[])
{
    const auto address = CynNet::Address{"192.168.0.102", 3000, CynNet::ConnectionType::UDP};
    const auto socket = CynNet::Socket{address.GetIP4Addresses()[0].first, address.GetIP4Addresses()[0].second};

    //socket.Send("Hello World", 11);
    std::cout << socket.Receive(128).data() << std::endl;

    return 0;
}

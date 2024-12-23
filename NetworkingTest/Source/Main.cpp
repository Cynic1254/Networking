#include <iostream>

#include "Address.hpp"


int main(int argc, const char* argv[])
{
    const auto address = CynNet::Address{"localhost", 80, CynNet::ConnectionType::UDP};

    std::cout << address.GetAddressName() << " Has these ip's:" << std::endl;

    for (auto& ip4_address : address.GetIP4Addresses())
    {
        std::cout << ip4_address.first.ToString() << " Type: " << (int)ip4_address.second << std::endl;
    }

    for (auto& ip6_address : address.GetIP6Addresses())
    {
        std::cout << ip6_address.first.ToString() << " Type: " << (int)ip6_address.second << std::endl;
    }

    return 0;
}

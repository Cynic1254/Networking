#include "library.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

void hello() {
    WSAData wsaData;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return;
    }

    std::cout << wsaData.wHighVersion << std::endl;
    std::cout << wsaData.szDescription << std::endl;

    addrinfo *result = nullptr;
    addrinfo *ptr = nullptr;
    addrinfo hints{};

    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo("localhost", "80", &hints, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        return;
    }

    auto socketVal = INVALID_SOCKET;
    ptr = result;
    socketVal = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (socketVal == INVALID_SOCKET) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return;
    }
}

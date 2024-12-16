#include "wsadata.hpp"

#include <stdexcept>
#include <winsock2.h>

#include "tools/exception.hpp"

namespace CynNet {
WSADATA wsaData;
bool intialized = false;

void CynNet::Init() {
    if (intialized == false) {
        int err = WSAStartup(MAKEWORD(2, 2), &wsaData);

        if (err != 0) {
            switch (err) {
                case WSASYSNOTREADY:
                    throw WSANotReadyException{
                        "The underlying network subsystem is not ready for network communication."};
                case WSAVERNOTSUPPORTED:
                    throw WSAUnsupportedVersionException{
                        "The version of Windows Sockets support requested is not provided by this particular Windows "
                        "Sockets implementation. Minimum version: 2.2"};
                case WSAEINPROGRESS:
                    throw WSAInProgressException{
                        "A blocking Windows Sockets 1.1 operation is in progress. try again later"};
                case WSAEPROCLIM:
                    throw WSAProcessLimitException{
                        "A limit on the number of tasks supported by the Windows Sockets implementation has been "
                        "reached. "};
                default:
                    throw WSAInvalidDataException{"Invalid WinSock Error"};
            }
        }

        intialized = true;
    }
}
void Close() {
    if (intialized != false) {
        int err = WSACleanup();
        if (err != 0) {
            switch (err) {
                case WSANOTINITIALISED:
                    throw WSANotInitializedException{
                        "A successful WSAStartup call must occur before using this function."};
                case WSAENETDOWN:
                    throw WSAEnetDownException{"The network subsystem has failed."};
                case WSAEINPROGRESS:
                    throw WSAInProgressException{
                        "A blocking Windows Sockets 1.1 operation is in progress. try again later"};
                default:
                    throw WSAInvalidDataException{"Invalid WinSock Error"};
            }
        }
        intialized = false;
    }
}

} // namespace CynNet

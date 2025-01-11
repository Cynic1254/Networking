#ifndef SOCKETS_HPP
#define SOCKETS_HPP
#include <winsock2.h>

#include "Address.hpp"

namespace CynNet
{
    /// @brief Base socket class, this class is just responsible for creating the underlying socket object
    class Socket
    {
    public:
        Socket(const IP& ip, ConnectionType protocol);
        virtual ~Socket();

    protected:
        Socket(SOCKET socket, IP ip, ConnectionType protocol);

        IP ip;
        ConnectionType protocol;
        SOCKET socketObject = INVALID_SOCKET;
    };

    /// @brief UDP socket class for making UDP type connections
    class UDPSocket : private Socket
    {
    public:
        explicit UDPSocket(const IP& ip);

        /// @brief Send data over UDP connection
        /// @param data Data to send
        /// @return Number of bytes that actually got send
        [[nodiscard]] int Send(const std::vector<char>& data) const;
        /// @brief Read the receive buffer or wait for a message to come in, if no messages are in the buffer this function will block
        /// @param sizeLimit Maximum size of the message that can be received, if there are more bytes in the message the overflow will be discarded. A value of <=0 means no limit and will always fit the whole message
        /// @return Bytes received
        [[nodiscard]] std::vector<char> Receive(u_long sizeLimit = 0) const;
        /// @brief Peek how many bytes are in the buffer waiting to be read using 'Receive', useful for preventing receive from blocking
        /// @return The number of pending bytes in the buffer
        [[nodiscard]] u_long GetPendingBytes() const;
    };

    /// @brief TCP socket class for making TCP type connections
    class TCPSocket : private Socket
    {
    public:
        explicit TCPSocket(const IP& ip);

        /// @brief Send data over TCP connection
        /// @param data Data to send
        /// @return Number of bytes that actually got send
        [[nodiscard]] int Send(const std::vector<char>& data) const;
        /// @brief Read the receive buffer or wait for data to come in, if there is no data in the buffer this function will block
        /// @param sizeLimit Maximum size of the data that can be received. A value of <=0 means no limit and will always fit all the data
        /// @return Bytes received
        [[nodiscard]] std::vector<char> Receive(u_long sizeLimit = 0) const;
        /// @brief Peek how many bytes are in the buffer waiting to be read using 'Receive', useful for preventing receive from blocking
        /// @return The number of pending bytes in the buffer
        [[nodiscard]] u_long GetPendingBytes() const;

    private:
        friend class TCPServer;
        TCPSocket(SOCKET socket, const IP& ip);
    };

    /// @brief TCP server class for accepting TCP connections
    class TCPServer : private Socket
    {
    public:
        explicit TCPServer(const IP& ip, int maxConnections = SOMAXCONN);

        /// @brief Empty the buffer of incoming connections and create a socket for each one. If no connections are pending Accept will block until there are
        /// @return Vector containing TCP sockets bound to all the pending connections
        [[nodiscard]] std::vector<TCPSocket> Accept() const;
        /// @brief Peek the number of pending connections, Useful for preventing Accept from blocking
        /// @return The number of connections waiting to be accepted
        [[nodiscard]] int GetPendingConnections() const;
    };
} // namespace CynNet

#endif  // SOCKETS_HPP
 

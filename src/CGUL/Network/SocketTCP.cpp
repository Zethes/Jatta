// C++ General Utility Library (mailto:cgul@zethes.com)
// Copyright (C) 2012-2014, Joshua Brookover and Amber Thrall
// All rights reserved.

/** @file SocketTCP.cpp
 *  @brief Implements Network::SocketTCP
 */

#include "SocketTCP.hpp"
#include "../Exceptions/NetworkException.hpp"

#ifdef OpenSSL_FOUND
#   include <openssl/rand.h>
#   include <openssl/ssl.h>
#   include <openssl/err.h>
#endif

#ifndef DOXYGEN
namespace CGUL
{
    namespace Network
    {
        void __cgul_network_initiate();
        void __cgul_network_clean();
        UInt8 __cgul_network_error_reason();
    }
}
#endif

/** @brief Makes the socket a non-blocking socket.
 *  @details This happens to all sockets created.  This class does not supported blocking sockets.
 *  @returns True if successful, false otherwise.
 */
bool CGUL::Network::SocketTCP::MakeNonBlocking()
{
#   ifdef CGUL_WINDOWS
    u_long uNonBlocking = 1;
    return (ioctlsocket(sock, FIONBIO, &uNonBlocking) == 0);
#   else
    return (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) != -1);
#   endif
}

/** @brief Turns off the Nagle Algorithm which removes a small delay in sending and receiving.
 *  @returns True if successful, false otherwise.
 */
bool CGUL::Network::SocketTCP::MakeNoDelay()
{
    // Turn off the Nagle Algorithm which will increase the socket's send and receive speed.
    int flag = 1;
    int result = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));

    // Check if there was a problem in doing so.
    return (result == 0);
}

CGUL::Network::SocketTCP::SocketTCP()
{
    sock = INVALID_SOCKET;
    __cgul_network_initiate();

#   ifdef OpenSSL_FOUND
    connectionSecure = false;
#   endif
}

CGUL::Network::SocketTCP::~SocketTCP()
{
}

/** @brief Connects to a server on a given ip and port.
 *  @param ip The IP address to connect to.
 *  @param port The port number.
 */
void CGUL::Network::SocketTCP::Connect(const IPAddress& ip, unsigned short port)
{
    // Check that the IP is valid
    if (!ip.IsValid())
    {
        throw NetworkException(NetworkExceptionCode::FAILED_CONNECT, NetworkExceptionReason::ADDRESS_INVALID);
    }

    // Create a hints variable used to determine the connection configuration.
    struct addrinfo hints;
    memset(&hints, 0, sizeof(addrinfo));

    // Check if the IP is an IPv4 or IPv6.
    if (ip.GetType() == IPAddressType::IPV4)
    {
        // Use IPv4.
        hints.ai_family = AF_INET;
    }
    else
    {
        // Use IPv6.
        hints.ai_family = AF_INET6;
    }

    // We're setting up a TCP/IP connection, which is a STREAM socket.
    hints.ai_socktype = SOCK_STREAM;

    // Convert the port into a string.
    char portString[6];
#   ifdef MSVC
    sprintf_s(portString, "%d", port);
#   else
    sprintf(portString, "%d", port);
#   endif

    // Get the address info using the hints.
    int status;
    addrinfo* result;
    if ((status = getaddrinfo(ip.ToString().GetCString(), portString, &hints, &result)) != 0)
    {
        throw NetworkException(NetworkExceptionCode::FAILED_CONNECT, NetworkExceptionReason::UNKNOWN, SystemCode::UseAddrInfo(status));
    }

    // Create the socket.  Because our hints are so strict, we don't have to worry about looping
    // through the linked list.  We should be able to trust that the first result is what we want.
    sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        throw NetworkException(NetworkExceptionCode::FAILED_CONNECT, NetworkExceptionReason::FAILED_CREATE_SOCKET, SystemCode::CheckNetwork());
    }

    // Make the connection.
    if (::connect(sock, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
    {
        freeaddrinfo(result);
        Close();
        throw NetworkException(NetworkExceptionCode::FAILED_CONNECT, NetworkExceptionReason::FAILED_CONNECT_CALL, SystemCode::CheckNetwork());
    }

    // Make a non-blocking socket.
    if (!MakeNonBlocking())
    {
        freeaddrinfo(result);
        Close();

        throw NetworkException(NetworkExceptionCode::FAILED_CONNECT, NetworkExceptionReason::FAILED_NONBLOCKING, SystemCode::CheckNetwork());
    }

    // Turn off the Nagle Algorithm to increase speed.
    if (!MakeNoDelay())
    {
        freeaddrinfo(result);
        Close();

        throw NetworkException(NetworkExceptionCode::FAILED_CONNECT, NetworkExceptionReason::FAILED_NO_DELAY, SystemCode::CheckNetwork());
    }

    // Free up the address info linked list.
    freeaddrinfo(result);
}

/** @brief Starts listening for clients on a specified port.
 *  @param port The port number.
 *  @param ipv4 Whether to use the local ipv4 address or not.  Will use an ipv6 address otherwise.
 *  @param backlog How many clients can wait to be accepted.  Defaults to 10.
 */
void CGUL::Network::SocketTCP::Listen(unsigned short port, bool ipv4, int backlog)
{
    // Convert the port into a string.
    char portString[6];
#   ifdef MSVC
    sprintf_s(portString, "%d", port);
#   else
    sprintf(portString, "%d", port);
#   endif

    // Create a hints variable used to determine the connection configuration.
    struct addrinfo hints;
    memset(&hints, 0, sizeof(addrinfo));

    // Check if IPv4 or IPv6.
    if (ipv4)
    {
        // Use IPv4.
        hints.ai_family = AF_INET;
    }
    else
    {
        // Use IPv6.
        hints.ai_family = AF_INET6;
    }

    // We're setting up a TCP/IP connection, which is a STREAM socket.
    hints.ai_socktype = SOCK_STREAM;

    // Pick the address for us (we'll use a local address)
    hints.ai_flags = AI_PASSIVE;

    // Get the address info using the hints.
    int status;
    addrinfo* result;
    if ((status = getaddrinfo(NULL, portString, &hints, &result)) != 0)
    {
        throw NetworkException(NetworkExceptionCode::FAILED_LISTEN, NetworkExceptionReason::NO_NETWORK_INTERFACE, SystemCode::UseAddrInfo(status));
    }

    // Create the socket.
    sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock == INVALID_SOCKET)
    {
        freeaddrinfo(result);
        throw NetworkException(NetworkExceptionCode::FAILED_LISTEN, NetworkExceptionReason::FAILED_CREATE_SOCKET, SystemCode::CheckNetwork());
    }

    // Bind the socket to the port.
    if (bind(sock, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
    {
        freeaddrinfo(result);
        Close();
        throw NetworkException(NetworkExceptionCode::FAILED_LISTEN, NetworkExceptionReason::FAILED_BIND_PORT, SystemCode::CheckNetwork());
    }

    // Start listening like the champ that we are.
    if (::listen(sock, backlog) == SOCKET_ERROR)
    {
        freeaddrinfo(result);
        Close();
        throw NetworkException(NetworkExceptionCode::FAILED_LISTEN, NetworkExceptionReason::FAILED_LISTEN_CALL, SystemCode::CheckNetwork());
    }

    // Make a non-blocking socket.
    if (!MakeNonBlocking())
    {
        freeaddrinfo(result);
        Close();
        throw NetworkException(NetworkExceptionCode::FAILED_LISTEN, NetworkExceptionReason::FAILED_NONBLOCKING, SystemCode::CheckNetwork());
    }

    // Turn off the Nagle Algorithm to increase speed.
    if (!MakeNoDelay())
    {
        freeaddrinfo(result);
        Close();
        throw NetworkException(NetworkExceptionCode::FAILED_LISTEN, NetworkExceptionReason::FAILED_NO_DELAY, SystemCode::CheckNetwork());
    }

    // Free up the address info linked list.
    freeaddrinfo(result);
}

bool CGUL::Network::SocketTCP::Accept(SocketTCP* socket)
{
    // Check if the socket is valid before we continue.
    if (sock == INVALID_SOCKET)
    {
        throw NetworkException(NetworkExceptionCode::FAILED_ACCEPT, NetworkExceptionReason::SOCKET_INVALID);
    }

    // Try to accept an incoming client.
    if ((socket->sock = ::accept(sock, NULL, NULL)) == INVALID_SOCKET)
    {
#       ifdef CGUL_WINDOWS
        int error = WSAGetLastError();
        if (error == WSAEWOULDBLOCK)
#       else
        int error = errno;
        if (error == EWOULDBLOCK)
#       endif
        {
            return false;
        }
        else
        {
            throw NetworkException(NetworkExceptionCode::FAILED_ACCEPT, NetworkExceptionReason::UNKNOWN, SystemCode::UseNetwork(error));
        }
    }

    // Make a non-blocking socket.
    if (!socket->MakeNonBlocking())
    {
        socket->Close();
        throw NetworkException(NetworkExceptionCode::FAILED_ACCEPT, NetworkExceptionReason::FAILED_NONBLOCKING, SystemCode::CheckNetwork());
    }

    // Turn off the Nagle Algorithm to increase speed.
    if (!socket->MakeNoDelay())
    {
        socket->Close();
        throw NetworkException(NetworkExceptionCode::FAILED_ACCEPT, NetworkExceptionReason::FAILED_NO_DELAY, SystemCode::CheckNetwork());
    }

    return true;
}


#ifdef OpenSSL_FOUND
void CGUL::Network::SocketTCP::ConnectSSL(const IPAddress& ip, unsigned short port)
{
    sslHandle = NULL;
    sslContext = NULL;
    Connect(ip, port);

    if (sock)
    {
        SSL_load_error_strings();
        SSL_library_init();

        sslContext = SSL_CTX_new(SSLv23_client_method());
        if (sslContext == NULL)
        {
            throw NetworkException(NetworkExceptionCode::FAILED_SSL_SETUP, NetworkExceptionReason::FAILED_CTX_NEW);
        }

        sslHandle = SSL_new(sslContext);
        if (sslHandle == NULL)
        {
            throw NetworkException(NetworkExceptionCode::FAILED_SSL_SETUP, NetworkExceptionReason::FAILED_SSL_NEW);
        }

        if (!SSL_set_fd(sslHandle, sock))
        {
            throw NetworkException(NetworkExceptionCode::FAILED_SSL_SETUP, NetworkExceptionReason::FAILED_SSL_FD);
        }

        while (true)
        {
            int resp = SSL_connect(sslHandle);
            if (resp == 1)
            {
                break;
            }

            int error = SSL_get_error(sslHandle, resp);

            if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE)
            {
                //Handle these errors then try again.
                //Just waiting it out seems to work.
            }
            else
            {
                throw NetworkException(NetworkExceptionCode::FAILED_SSL_SETUP, NetworkExceptionReason::FAILED_SSL_CONNECT);
            }
        }
    }
    else
    {
        throw NetworkException(NetworkExceptionCode::FAILED_SSL_SETUP, NetworkExceptionReason::SOCKET_INVALID);
    }

    connectionSecure = true;
}

void CGUL::Network::SocketTCP::ListenSSL(unsigned short port, bool ipv4, int backlog)
{
    SSL_METHOD * method;
    SSL_CTX * ctx;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    method = (SSL_METHOD*)SSLv23_server_method();
    ctx = SSL_CTX_new(method);
    if (ctx == NULL)
    {
        throw NetworkException(NetworkExceptionCode::FAILED_SSL_SETUP, NetworkExceptionReason::FAILED_CTX_NEW);
    }
    Listen(port, ipv4, backlog);
}
#endif


void CGUL::Network::SocketTCP::Close()
{
#   ifdef CGUL_WINDOWS
    closesocket(sock);
#   else
    ::close(sock);
#   endif
    sock = INVALID_SOCKET;

#   ifdef OpenSSL_FOUND
    if (connectionSecure)
    {
        if (sslHandle)
        {
            SSL_shutdown(sslHandle);
            SSL_free(sslHandle);
        }
        if (sslContext)
        {
            SSL_CTX_free(sslContext);
        }

        connectionSecure = false;
    }
#   endif
}

/** @brief Checks if the socket is still connected to the remote host.
 *  @returns True if the connection is still active, false otherwise.
 */
bool CGUL::Network::SocketTCP::IsConnected()
{
#   ifdef OpenSSL_FOUND
    if (connectionSecure)
    {
        char data;
        if (sock == INVALID_SOCKET)
        {
            return false;
        }
        if (SSL_peek(sslHandle, &data, 1) == 0)
        {
            sock = INVALID_SOCKET;
            return false;
        }
        return true;
    }
#   endif

    // Check if we've already determined the connection is dead.  If so, we can go ahead and return.
    if (sock == INVALID_SOCKET)
    {
        return false;
    }

    // Check if recv is returning 0.  In that case, the remote host has disconnected gracefully.
    char data;

    int result = recv(sock, &data, 1, MSG_PEEK);

    if (result == 0)
    {
        sock = INVALID_SOCKET;
        return false;
    }

    if (result == -1)
    {
        // May get a "would block" error, meaning there wasn't actually a problem, the call would
        // normally block. In this case the socket is still connected.
#       ifdef CGUL_WINDOWS
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK)
#       else
        int error = errno;
        if (error != EWOULDBLOCK)
#       endif
        {
            return false;
        }
    }

    return true;
}

CGUL::Network::IPAddress CGUL::Network::SocketTCP::GetIP()
{
    if (!IsConnected())
    {
        // Return an invalid IP address
        return IPAddress();
    }

    struct sockaddr_storage _Addr;
    socklen_t _Length = sizeof(_Addr);
    getpeername(sock, (struct sockaddr*)&_Addr, &_Length);
    if (_Addr.ss_family == AF_INET) // ipv4
    {
        return IPAddress((UInt32)((struct sockaddr_in*)&_Addr)->sin_addr.s_addr);
    }
    else // ipv6
    {
        UInt64 address[2];
        memcpy(&address, &((struct sockaddr_in6*)&_Addr)->sin6_addr, sizeof(address));
        return IPAddress(address);
    }
}

/** @brief Sends data over the network.
 *  @param data An array of bytes to send over the network.
 *  @param size The size of the array.
 *  @returns The number of bytes that were sent.
 */
int CGUL::Network::SocketTCP::Send(const void* data, unsigned int size)
{
#   ifdef OpenSSL_FOUND
    if (connectionSecure)
    {
        int amount;
        if (sock == INVALID_SOCKET)
        {
            return false;
        }

        if ((amount = SSL_write(sslHandle, (const char*)data, size)) <= 0)
        {
            throw NetworkException(NetworkExceptionCode::FAILED_SEND, NetworkExceptionReason::UNKNOWN);
        }
        return amount;
    }
#   endif

    // Check if the socket is valid before we continue.
    if (sock == INVALID_SOCKET)
    {
        throw NetworkException(NetworkExceptionCode::FAILED_SEND, NetworkExceptionReason::SOCKET_INVALID);
    }

    // Send normally
    int amount;
    if ((amount = ::send(sock, (const char*)data, size, 0)) == SOCKET_ERROR)
    {
#       ifdef CGUL_WINDOWS
        int error = WSAGetLastError();
        if (error == WSAENOTCONN)
        {
            throw NetworkException(NetworkExceptionCode::FAILED_SEND, NetworkExceptionReason::SOCKET_NOT_CONNECTED, SystemCode::UseNetwork(error));
        }
#       else
        int error = errno;
        if (error == ENOTCONN)
        {
            throw NetworkException(NetworkExceptionCode::FAILED_SEND, NetworkExceptionReason::SOCKET_NOT_CONNECTED, SystemCode::UseNetwork(error));
        }
#       endif
        throw NetworkException(NetworkExceptionCode::FAILED_SEND, NetworkExceptionReason::UNKNOWN, SystemCode::UseNetwork(error));
    }
    return amount;
}

/** @brief Receives data over the network.
 *  @param data The buffer the data will be written into.
 *  @param size The size of the data array.
 *  @returns The number of bytes that were received, or 0 if there was nothing to be received.
 */
int CGUL::Network::SocketTCP::Receive(void* data, unsigned int size)
{
#   ifdef OpenSSL_FOUND
    if (connectionSecure)
    {
        int amount;
        if (sock == INVALID_SOCKET)
        {
            return false;
        }

        while (true)
        {
            amount = SSL_read(sslHandle, (char*)data, size);

            if (amount == 0)
            {
                Close();
                return 0;
            }
            else if (amount > 0)
            {
                return amount;
            }

            int error = SSL_get_error(sslHandle, amount);

            if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE)
            {
                //Handle these errors then try again.
                //Just waiting it out seems to work.
            }
            else
            {
                throw NetworkException(NetworkExceptionCode::FAILED_RECEIVE, NetworkExceptionReason::UNKNOWN, SystemCode::UseSSL(error));
            }
        }
    }
#   endif

    // Check if the socket is valid before we continue.
    if (sock == INVALID_SOCKET)
    {
        throw NetworkException(NetworkExceptionCode::FAILED_RECEIVE, NetworkExceptionReason::SOCKET_INVALID);
    }

    // Receive normally.
    int amount;
    if ((amount = ::recv(sock, (char*)data, size, 0)) == SOCKET_ERROR)
    {
        // Check if recv failed because of a WOULDBLOCK error.  This basically means that there was
        // nothing to be received.  In that case, just return 0.  Otherwise, there was an error.
#       ifdef CGUL_WINDOWS
        int error = WSAGetLastError();
        if (error == WSAEWOULDBLOCK)
#       else
        int error = errno;
        if (error == EWOULDBLOCK)
#       endif
        {
            return 0;
        }
        else
        {
            UInt8 errorReason = __cgul_network_error_reason();

            // Certain errors result in the connection being lost.
            // Need to kill the connection in those cases.
            switch (errorReason)
            {
                case NetworkExceptionReason::CONNECTION_ABORTED:
                case NetworkExceptionReason::CONNECTION_RESET:
                {
                    Close();
                    break;
                }
            }
            throw NetworkException(NetworkExceptionCode::FAILED_RECEIVE, errorReason, SystemCode::UseNetwork(error));
        }
    }

    // Check if recv returned 0, if so, the remote socket disconnected gracefully.
    if (amount == 0)
    {
        Close();
        return 0;
    }
    else
    {
        return amount;
    }
}

int CGUL::Network::SocketTCP::Peek(void* data, unsigned int size)
{
#   ifdef OpenSSL_FOUND
    if (connectionSecure)
    {
        int amount;
        if (sock == INVALID_SOCKET)
        {
            return false;
        }

        while (true)
        {
            amount = SSL_peek(sslHandle, (char*)data, size);

            if (amount == 0)
            {
                Close();
                return 0;
            }
            else if (amount > 0)
            {
                return amount;
            }

            int error = SSL_get_error(sslHandle, amount);

            if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE)
            {
                //Handle these errors then try again.
                //Just waiting it out seems to work.
            }
            else
            {
                throw NetworkException(NetworkExceptionCode::FAILED_PEEK, NetworkExceptionReason::UNKNOWN, SystemCode::UseSSL(error));
            }
        }
    }
#   endif
    // Check if the socket is valid before we continue.
    if (sock == INVALID_SOCKET)
    {
        throw NetworkException(NetworkExceptionCode::FAILED_PEEK, NetworkExceptionReason::SOCKET_INVALID);
    }

    //Peek normally
    int amount;
    if ((amount = ::recv(sock, (char*)data, size, MSG_PEEK)) == SOCKET_ERROR)
    {
        // Check if recv failed because of a WOULDBLOCK error.  This basically means that there was
        // nothing to be received.  In that case, just return 0.  Otherwise, there was an error.
#       ifdef CGUL_WINDOWS
        int error = WSAGetLastError();
        if (error == WSAEWOULDBLOCK)
#       else
        int error = errno;
        if (error == EWOULDBLOCK)
#       endif
        {
            return 0;
        }
        else
        {
            throw NetworkException(NetworkExceptionCode::FAILED_PEEK, NetworkExceptionReason::UNKNOWN, SystemCode::UseNetwork(error));
        }
    }

    // Check if recv returned 0, if so, the remote socket disconnected gracefully.
    if (amount == 0)
    {
        Close();
        return 0;
    }
    else
    {
        return amount;
    }
}

#ifndef SOCKET_PLATFORM_H
#define SOCKET_PLATFORM_H

// Definition for WinX64
// -------------------------------------------
#ifdef _WIN64

// Includes
#include <WinSock2.h>
#include <ws2tcpip.h>

// Types
#define PlatformSocket SOCKET
typedef int socklen_t;

// Values
#define PlatformInvalidSocket	INVALID_SOCKET

// Functions
#define PlatformCloseSocket(x) ::closesocket(x)

static WSADATA g_wsaData;

inline int platform_socket_error()
{
	return WSAGetLastError();
}

inline void platform_init_sockets()
{
	WSAStartup(MAKEWORD(2, 2), &g_wsaData);
}

inline void platform_cleanup_sockets()
{
	WSACleanup();
}

inline int platform_recvfrom(PlatformSocket socket, void* buffer, int bufferSize, int flags, struct sockaddr_in* sockAddr, socklen_t* sockAddrLen)
{
	return recvfrom(socket,
		static_cast<char*>(buffer),
		bufferSize,
		flags,
		reinterpret_cast<struct sockaddr *>(sockAddr),
		sockAddrLen);
}

inline int platform_sendto(PlatformSocket socket, void* buffer, int bufferSize, int flags, struct sockaddr_in& sockAddr)
{
	return sendto(socket,
		static_cast<char*>(buffer),
		bufferSize,
		flags,
		reinterpret_cast<struct sockaddr *>(&sockAddr),
		sizeof sockAddr);
}

inline int platform_send(PlatformSocket socket, void* buffer, int bufferSize, int flags)
{
	return send(socket,
		static_cast<char*>(buffer),
		bufferSize,
		MSG_WAITALL);
}

inline int platform_recv(PlatformSocket socket, void* buffer, int bufferSize, int flags)
{
	return recv(socket,
		static_cast<char*>(buffer),
		bufferSize,
		MSG_WAITALL);
}

#endif

// Definition for Linux
// -------------------------------------------
#ifdef __linux__

// Includes
#include <sys/socket.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>

// Types
#define PlatformSocket	int32_t

// Values
#define PlatformInvalidSocket	-1

// Functions
#define PlatformCloseSocket(x) ::close(x)

inline int platform_recvfrom(PlatformSocket socket, void* buffer, int bufferSize, int flags, struct sockaddr_in* sockAddr, socklen_t* sockAddrLen)
{
	return recvfrom(socket,
		buffer,
		bufferSize,
		MSG_WAITALL,
		reinterpret_cast<struct sockaddr *>(sockAddr),
		sockAddrLen);
}

inline int platform_sendto(PlatformSocket socket, void* buffer, int bufferSize, int flags, struct sockaddr_in& sockAddr)
{
	return sendto(socket,
		buffer,
		bufferSize,
		MSG_WAITALL,
		reinterpret_cast<struct sockaddr *>(&sockAddr),
		sizeof sockAddr);
}

inline int platform_send(PlatformSocket socket, void* buffer, int bufferSize, int flags)
{
	return send(socket,
		buffer,
		bufferSize,
		MSG_WAITALL);
}

inline int platform_recv(PlatformSocket socket, void* buffer, int bufferSize, int flags)
{
	return recv(socket,
		buffer,
		bufferSize,
		MSG_WAITALL);
}

inline void platform_init_sockets()
{
	// Nothing to do
}

inline void platform_cleanup_sockets()
{
	// Nothing to do
}

inline int platform_socket_error()
{
	return errno;
}

#endif

#endif // SOCKET_PLATFORM_H
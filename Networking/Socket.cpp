#include "Socket.hpp"
#include "Net.hpp"
#include "Engine/Platform/WinCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "cstdint"
#include <Engine/Core/StringUtils.hpp>
#include <Engine/Core/EngineCommon.hpp>
void Socket::Close()
{
	if (!IsClosed())
	{
		::closesocket(m_handle);
		m_handle = INVALID_SOCKET;
	}
}

bool Socket::CheckforFatalError()
{
	int error = WSAGetLastError();
	switch (error)
	{
		case 0 :
			return true;
		case WSAEWOULDBLOCK:
			return false;
		default:
			DebuggerPrintf(Stringf("Disconnected because of this error : %i", error).c_str());
			return true;
	}
}

bool Socket::IsClosed() const
{
	return m_handle == INVALID_SOCKET;
}

Socket::Socket()
{
	m_handle = INVALID_SOCKET;
}

void Socket::SetBlocking(bool isBlocking)
{
	u_long non_Blocking = isBlocking ? 0 : 1;
	::ioctlsocket(m_handle, FIONBIO, &non_Blocking);
}

bool TCPConnection::Connect(NetAddress const& addr)
{
	UNUSED((void) addr);
	//sockaddr_in ipv4;
	//unsigned int uintAddr = addr.address;
	//uint16_t port = addr.port;
	//ipv4.sin_family = AF_INET;
	//ipv4.sin_addr.S_un.S_addr = ::htonl(uintAddr);
	//ipv4.sin_port = ::htons(port);

	////Create socket
	//SOCKET mySocket = ::socket(AF_INET, SOCK_STREAM , IPPROTO_TCP);
	//if (mySocket == INVALID_SOCKET)
	//{
	//	//int error = ::WSAGetLastError();
	//	ERROR_AND_DIE("Failed to create socket");
	//}

	//// this will block until failure or success
	//int result = ::connect(mySocket, (sockaddr*) &ipv4, (int)sizeof(ipv4));
	//if (result == SOCKET_ERROR)
	//{
	//	//int error2 = ::WSAGetLastError();
	//	::closesocket(mySocket);
	//	return false;
	//}

	//m_handle = mySocket;
	//m_address = addr;
	return true;
}

size_t TCPConnection::Send(void const* data, size_t const dataSize)
{
	if (IsClosed()) {return 0;} 
	
	int bytesSent = ::send(m_handle, (char const*) data, (int) dataSize, 0);
	if (bytesSent > 0)
	{
		GUARANTEE_OR_DIE(bytesSent == dataSize, "Data sent incorrectly");
		return bytesSent;
	}
	else if (bytesSent == 0) // Special case socket closed cleanly
	{
		Close();
	}
	else
	{
	/*	int result = ::WSAGetLastError();*/
		// Socket error -  Handle it
		Close();
		return 0;
	}
	return 0;
}

size_t TCPConnection::Receive(void* buffer, size_t bufferSize)
{
	int bytesRead = ::recv(m_handle, (char*)buffer, (int)bufferSize, 0);
	if (bytesRead == 0)
	{
		Close();
	}
	if (bytesRead > 0)
	{
		return (size_t)bytesRead;
	}
	else
	{
		//ERROR, Dont just close, Handle it later
		//Close();
	}
	return size_t();
}


bool TCPConnection::CheckForConnection()
{
	if (IsConnected()) {
		return true;
	}

	if (IsClosed()) {
		return false;
	}

	WSAPOLLFD poll;
	poll.fd = m_handle;
	poll.events = POLLWRNORM;

	int result = WSAPoll(&poll, 1, 0);
	if (result == SOCKET_ERROR) {
		Close();
		return false;
	}

	if ((poll.revents & POLLHUP) != 0) { // the socket hung up on
		Close();
		return false;
	}

	if ((poll.revents & POLLWRNORM) != 0) {
		// mark as connected
		return true;
	}
	return false;
}

bool TCPConnection::IsConnected()
{
	return m_connectionState == ConnectionState::Connected;
}

TCPConnection* TCPServer::Accept()
{
	if (IsClosed())
	{
		return nullptr;
	}
	sockaddr_storage addr;
	int addrLen = sizeof(addr);

	SocketHandle handle = ::accept(m_handle, (sockaddr*) &addr , &addrLen);
	if (handle == INVALID_SOCKET)
	{
		return nullptr;
	}
	//make sure it is the right address family (ipv4)
	if (addr.ss_family != AF_INET)
	{
		Close();
		return nullptr;
	}

	sockaddr_in* ipv4= (sockaddr_in*) &addr;

	NetAddress netAddress;
	netAddress.address = ::ntohl(ipv4->sin_addr.S_un.S_addr);
	netAddress.port = ::ntohs(ipv4->sin_port);

	TCPConnection* conn = new TCPConnection();
	conn->m_handle = handle;
	conn->m_address = netAddress;
	return conn;
}

bool TCPServer::Host(uint16_t service, uint32_t backlog)
{
	NetAddress hostingAddress;
	hostingAddress.address = INADDR_ANY; // Any IP that refers to my machine
	hostingAddress.port = service;

	SOCKET mysocket = ::socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);

	if (mysocket == INVALID_SOCKET)
	{
		return false;
	}

	sockaddr_in ipv4;
	ipv4.sin_family = AF_INET;
	ipv4.sin_addr.S_un.S_addr = ::htonl(hostingAddress.address);
	ipv4.sin_port = ::htons((USHORT)hostingAddress.port);

	int result = ::bind(mysocket, (sockaddr*)&ipv4, (int) sizeof(ipv4));
	if (SOCKET_ERROR == result)
	{
		::closesocket(mysocket);
		return false;
	}
	m_handle = mysocket;
	m_address = hostingAddress;

	// now listen
	result = ::listen(m_handle, backlog); // backlog-> how many people can be queued to connect
	if (result == SOCKET_ERROR)
	{
		Close();
	}
	return true;
}

#pragma once
#include <cstdint>
#include "Engine/Networking/Net.hpp"
//struct NetAddress;
typedef uintptr_t SocketHandle;

enum class ConnectionState
{
	Connected,
	IsConnecting,
	Disconnected
};
struct RDCHeader
{
	uint16_t payloadSize;
};
#pragma pack(push, 1)
struct RDCPayload
{
	char isEcho;
	uint16_t message_size;
};
#pragma pack(pop)

class Socket
{
public:
	Socket();
	~Socket() { Close(); };
	void Close();
	bool CheckforFatalError();
	bool IsClosed() const;
	void SetBlocking(bool isBlocking);
public:
	SocketHandle m_handle;
	NetAddress m_address;
};

class TCPSocket : public Socket
{

};

class TCPConnection :public TCPSocket
{
	public:
	bool Connect(NetAddress const& addr);
	size_t Send(void const* data, size_t const dataSize);
	size_t Receive(void* buffer,size_t bufferSize);
	bool CheckForConnection();
	bool IsConnected();
	ConnectionState m_connectionState;
	size_t m_bytesReceieved;
	unsigned char m_connBuffer[1024];

};

class TCPServer : public TCPSocket
{
	public:
	TCPConnection* Accept();
	bool Host(uint16_t service, uint32_t backlog = 16);
};
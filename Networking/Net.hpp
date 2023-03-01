#pragma once
#include <string>
#include <vector>
#include <Engine/Math/AABB2.hpp>
class DevConsole;
class Renderer;
class TCPServer;
class TCPConnection;
struct DevConsoleLine;
struct NetConfig
{

};
struct RemoteConsoleLine
{
	std::string text;
	Rgba8 color;
	int frame;
	double timeStamp;
};
class NetSystem
{
	//----------------------CONSTRUCTOR-----------------------
public:
	NetSystem(NetConfig netConfig);
	~NetSystem();

	//----------------------MAIN FUNCTIONS---------------
public:
	void Startup();
	void BeginFrame();
	void EndFrame();
	void ShutDown();
	
private:
	NetConfig m_netConfig;

};
struct RemoteConsoleConfig
{
	DevConsole* console = nullptr;
};

struct NetAddress
{
	public:
		NetAddress(){};
		static std::vector<NetAddress> GetAllInternal(uint16_t port);
		std::string ToString();
		unsigned int address;
		uint16_t port;
	public:
		static NetAddress FromString(std::string const &str) ;
		static NetAddress GetLoopBack(uint16_t port);
};
class RemoteConsole
{
public:
	RemoteConsole(RemoteConsoleConfig& config) {
		m_config = config;
	}
public:
	void Startup();
	void Shutdown();
	
	bool GetIsServer();
	void AttemptToJoinOrHost(uint16_t port);
	void AttemptToJoin(NetAddress addressToJoin);
	void AttemptToHost(uint16_t port);
	void Update();
	void ProcessConnections();
	void DeleteConnections();
	void KickPlayerAtIndex(int index);
	void ProcessMessage(TCPConnection* connection, int index);
	bool CheckValidityOfCommand(std::string command);
	bool IsPlayerBanned(TCPConnection* conn);
	void AddLine(Rgba8 const& color, std::string const& text);
	void SendCommand(int connIndex , std::string const& command, bool isEcho);
	void Render(AABB2 maxBounds, Renderer* renderer);
	std::vector<TCPConnection*> GetConnections();
private:
	std::vector<RemoteConsoleLine>	m_lines;
	std::vector<NetAddress>			m_kickedAddresses;
	TCPServer*						m_server = nullptr;
	std::vector<TCPConnection*>		m_connections;
	std::vector<NetAddress>			m_internalAddresses;
	RemoteConsoleConfig				m_config;
	float							m_consoleTextHeight = 20.0f;
	float							m_consoleTextAspect = 0.6f;
	Rgba8							m_networkColor = Rgba8::GREEN;
	Rgba8							m_commandColor = Rgba8::WHITE;
	Rgba8							m_failedColor = Rgba8::RED;

};
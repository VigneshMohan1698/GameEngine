#include "Engine/Networking/Net.hpp"
#include "Engine/Platform/WinCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Networking/Socket.hpp"
#include <WS2tcpip.h>
#include <Engine/Math/VertexUtils.hpp>

extern EventSystem* g_theEventSystem;
NetSystem::NetSystem(NetConfig netConfig)
{
	m_netConfig = netConfig;
}

NetSystem::~NetSystem()
{
}

void NetSystem::Startup()
{

	WORD version  = MAKEWORD(2,2);
	WSADATA data;

	int error = ::WSAStartup(version, &data);
	GUARANTEE_OR_DIE(error == 0, "Failed to initialize network system");
}

void NetSystem::BeginFrame()
{
}

void NetSystem::EndFrame()
{
}

void NetSystem::ShutDown()
{
	::WSACleanup();
}

void RemoteConsole::Startup()
{
	uint16_t servicePort = 3121;
	m_internalAddresses = NetAddress::GetAllInternal(servicePort);

	for (int i = 0; i < m_internalAddresses.size(); ++i) {
		std::string text = Stringf((m_internalAddresses[i].ToString() + "\n").c_str());
		DebuggerPrintf(text.c_str());
	}

	//---------WHEN STARTING UP TRY TO FIND A HOST OR BECOME THE HOST
	AttemptToJoinOrHost(servicePort);
}

void RemoteConsole::Shutdown()
{
}

bool RemoteConsole::GetIsServer()
{
	return m_server != nullptr;
}

void RemoteConsole::AttemptToJoinOrHost(uint16_t port)
{
	TCPServer* server = new TCPServer();
	
	if (server->Host(port))
	{
		server->SetBlocking(false);
		m_server = server;
		unsigned int hostIndex = m_server->m_address.address;
		std::string hostText = Stringf(("Host Successful: " + m_internalAddresses[hostIndex].ToString()).c_str());
		AddLine(m_networkColor, hostText.c_str());
		AddLine(m_commandColor,"Internal Addresses");
		for (int i = 0; i < m_internalAddresses.size(); i++)
		{
			AddLine(m_commandColor,m_internalAddresses[i].ToString());
		}

	}
	else
	{
		AddLine(m_failedColor, "Failed to host");
		TCPConnection* conn = new TCPConnection();
		NetAddress loopBack = NetAddress::GetLoopBack(port);
		if (conn->Connect(loopBack))
		{
			//unsigned int joinIndex = conn->m_address.address;
			std::string joinText = Stringf(("Join Successful: " + conn->m_address.ToString()).c_str());
			AddLine(m_networkColor, joinText.c_str());

			//CONNECTING TO SERVER AND ADDING THE SERVER TO m_connections
			conn->SetBlocking(false);
			for (int i = 0; i < (int)m_connections.size(); i++)
			{
				if (m_connections[i] == nullptr)
				{
					m_connections[i] = conn;
					return;
				}
			}
			m_connections.push_back(conn);
		}
	}
}
void RemoteConsole::AttemptToJoin(NetAddress addressToJoin)
{
	TCPConnection* conn = new TCPConnection();
	if (conn->Connect(addressToJoin) && !conn->IsClosed())
	{
		//unsigned int joinIndex = conn->m_address.address;
		//CANT OUTPUT HERE IS AS IT IS CHECKED IN UPDATE
		std::string joinText = Stringf(("Attempting to Join Address: " + conn->m_address.ToString()).c_str());
		AddLine(m_networkColor, joinText.c_str());

		//CONNECTING TO SERVER AND ADDING THE SERVER TO m_connections
		conn->SetBlocking(false);
		for (int i = 0; i < (int)m_connections.size(); i++)
		{
			if (m_connections[i] == nullptr)
			{
				m_connections[i] = conn;
				return;
			}
		}

		m_connections.push_back(conn);
	}
	else
	{
		std::string joinText = Stringf(("Join Failed to address " + addressToJoin.ToString()).c_str());
		AddLine(m_failedColor, joinText);
	}
}

void RemoteConsole::AttemptToHost(uint16_t port)
{
	TCPServer* server = new TCPServer();
	//IF ITS ALREADY HOSTED CLOSE THAT AND OPEN NEW ONE
	if (m_server)
	{
		m_server->Close();
		if (server->Host(port))
		{
			server->SetBlocking(false);
			m_server = server;
			unsigned int hostIndex = m_server->m_address.address;
			std::vector<NetAddress> internalAddresses = NetAddress::GetAllInternal(port);
			std::string hostText = Stringf(("Host Successful: " + internalAddresses[hostIndex].ToString()).c_str());
			AddLine(m_networkColor, hostText.c_str());
		}
		else
		{
			std::string hostText = Stringf(("Host Failed"));
			AddLine(m_failedColor, hostText);
		}
	}
	else
	{
		if (server->Host(port))
		{
			server->SetBlocking(false);
			m_server = server;
			unsigned int hostIndex = m_server->m_address.address;
			std::vector<NetAddress> internalAddresses = NetAddress::GetAllInternal(port);
			std::string hostText = Stringf(("Host Successful: " + internalAddresses[hostIndex].ToString()).c_str());
			AddLine(m_networkColor, hostText.c_str());
		}
		else
		{
			std::string hostText = Stringf(("Host Failed"));
			AddLine(m_failedColor, hostText);
		}
	}
}

void RemoteConsole::KickPlayerAtIndex(int connIndex)
{
	if (connIndex < 0 || connIndex >= (int)m_connections.size())
	{ 
		AddLine(m_failedColor, "No Connection found at specified index");
		return;
	}
	if (m_connections[connIndex] != nullptr)
	{
		std::string hostText = Stringf(("Kicked Player at address : " + m_connections[connIndex]->m_address.ToString()).c_str());
		AddLine(m_networkColor, hostText.c_str());
		SendCommand(connIndex, "You have been a bad boy", true);
		m_kickedAddresses.push_back(m_connections[connIndex]->m_address);
		m_connections[connIndex]->Close();
		m_connections.erase(m_connections.begin() + connIndex);
	}

}
void RemoteConsole::Update()
{
	if (GetIsServer())
	{
		TCPConnection* cp = m_server->Accept();
		if (cp)
		{
			if (!IsPlayerBanned(cp))
			{
				std::string joinText = Stringf(("Joined : " + cp->m_address.ToString()).c_str());
				AddLine(m_networkColor, joinText);
				m_connections.push_back(cp);
			}
			else
			{
				cp->Close();
			}
		}
		
	}
	ProcessConnections();
}
void RemoteConsole::ProcessConnections()
{
	for (int i = 0; i < (int)m_connections.size(); i++)
	{
		TCPConnection* conn = m_connections[i];
		if (conn)
		{
			if (conn->m_bytesReceieved < 2)
			{
				conn->m_bytesReceieved += conn->Receive(conn->m_connBuffer + conn->m_bytesReceieved, 2 - conn->m_bytesReceieved);
			}
			if (conn->m_bytesReceieved >= 2)
			{
				uint16_t payloadSize = ::ntohs(*(uint16_t*)conn->m_connBuffer);
				size_t recvSize = payloadSize - (conn->m_bytesReceieved - 2); // 
				if (recvSize != 0)
				{
					conn->m_bytesReceieved += conn->Receive(conn->m_connBuffer + conn->m_bytesReceieved, recvSize);
					if (conn->m_bytesReceieved >= (payloadSize + 2))
					{
						ProcessMessage(conn, i);
						conn->m_bytesReceieved = 0;
					}
				}
			}
			if (conn->IsClosed())
			{
				m_connections.erase(m_connections.begin() + i);
				delete conn;
				--i;
			}
		}
	}
}
void RemoteConsole::DeleteConnections()
{
	for (int i = 0; i < (int)m_connections.size(); i++)
	{
		TCPConnection* conn = m_connections[i];
		if (conn)
		{
			conn->Close();
			m_connections.erase(m_connections.begin() + i);
			delete conn;
		}
	}
}

void RemoteConsole::ProcessMessage(TCPConnection* connection,int index)
{
	const char* messageReceived = nullptr;
	connection->m_connBuffer[connection->m_bytesReceieved] = NULL;
	byte* iter = connection->m_connBuffer;
	iter += sizeof(uint16_t);
	bool isEcho = *iter;
	iter++;
	iter += sizeof(uint16_t);
	messageReceived = (const char*)iter;
	std::string mess = "";
	if (isEcho)
	{
		mess = "Echo Received : ";
	}
	else
	{
		mess = "Command Received : ";
	} 
	AddLine(m_commandColor, mess + messageReceived);
	if (isEcho)
	{
		m_config.console->AddLine(m_commandColor, messageReceived);
	}
	else if (CheckValidityOfCommand(messageReceived))
	{
		m_config.console->AddLine(m_networkColor,messageReceived);
		int lineStart = (int)m_config.console->m_lines.size() - 1;
		g_theEventSystem->FireEvent(messageReceived);
		int lineEnd = (int)m_config.console->m_lines.size() - 1;
		if (lineEnd - lineStart >  1)
		{
			for (int i = lineStart; i <= lineEnd; i++)
			{
				SendCommand(index, m_config.console->m_lines[i].text, true);
			}
		}
	}
	else
	{
		m_config.console->AddLine(m_failedColor, "Invalid Command");
	}

}
bool RemoteConsole::IsPlayerBanned(TCPConnection* conn)
{
	for (int i = 0; i < (int)m_kickedAddresses.size(); i++)
	{	
		if (m_kickedAddresses[i].address == conn->m_address.address)
		{
			return true;
		}
	}
	return false;
}
bool RemoteConsole::CheckValidityOfCommand(std::string command)
{
	std::vector<std::string> eventNames;
	g_theEventSystem->GetRegisteredEventNames(eventNames);
	if (std::find(eventNames.begin(), eventNames.end(), command) != eventNames.end())
	{
		return true;
	}
	return false;
}
void RemoteConsole::AddLine(Rgba8 const& color, std::string const& text)
{
	RemoteConsoleLine line;
	line.text = text;
	line.color = color;
	line.timeStamp = GetCurrentTimeSeconds();
	m_lines.push_back(line);
}
void RemoteConsole::SendCommand(int connIndex, std::string const& command, bool isEcho)
{
	if(connIndex < 0 || connIndex>=(int)m_connections.size()) {return;}

	TCPConnection* cp = m_connections[connIndex];
	if (cp == nullptr) {return;}
	size_t payloadSize = 1 + 2 + command.size();

	RDCHeader header;
	header.payloadSize = ::htons((u_short)payloadSize);
	RDCPayload payload;
	payload.isEcho = isEcho;
	payload.message_size  = htons((uint16_t)(command.size() + 1));
	cp->Send(&header, sizeof(header));
	cp->Send(&payload, sizeof(payload));
	size_t bytesSent = cp->Send(command.c_str(), command.size() + 1);
	if ((int)bytesSent != 0)
	{
		std::string CommandLine = "";
		if (isEcho)
		{
			CommandLine  = "Echo Sent : ";
		}
		else
		{
			CommandLine = "Command Sent : ";
		}
		
		AddLine(m_commandColor, CommandLine + command.c_str());
	}
}


void RemoteConsole::Render(AABB2 maxBounds, Renderer* renderer)
{
	AABB2 bounds = maxBounds.GetFractionalAABB2(Vec2(0.4f, 0.4f), Vec2(0.98f, 0.98f));
	AABB2 connectionBounds = bounds.GetFractionalAABB2(Vec2(0.7f, 0.1f), Vec2(0.95f, 0.92f));
	std::vector<Vertex_PCU> devConsoleVerts;
	std::vector<Vertex_PCU> devConsoleQuad;
	BitmapFont* font = renderer->CreateOrGetBitmapFont("Data/Images/SquirrelFixedFont");


	AABB2 textBounds;
	textBounds.m_mins.x = bounds.m_mins.x + (bounds.m_maxs.x - bounds.m_mins.x) * 0.002f;
	textBounds.m_mins.y = bounds.m_mins.y + (bounds.m_maxs.y - bounds.m_mins.y) * 0.002f;

	textBounds.m_maxs.y = bounds.m_maxs.y - (bounds.m_maxs.y - bounds.m_mins.y) * 0.002f;
	textBounds.m_maxs.x = bounds.m_maxs.x - (bounds.m_maxs.x - bounds.m_mins.x) * 0.002f;
	//AddVertsForAABB2D(devConsoleQuad, bounds, Rgba8(0, 0, 0, 200));

	for (int i = (int)m_lines.size() - 1; i >= 0; i--)
	{
		font->AddVertsForTextInBox2D(devConsoleVerts, textBounds, m_consoleTextHeight, m_lines[i].text, m_lines[i].color,
			m_consoleTextAspect, Vec2(0.0f, 0.0f), TextBoxMode::SHRINK_TO_FIT, 999999, 5.0f);
		textBounds.m_mins.y += m_consoleTextHeight;
	}

	//-----------------------------REMOTE CONSOLE BORDERS-------------------------------------------
	AABB2 borderBounds;

	borderBounds.m_mins = bounds.m_mins;
	borderBounds.m_maxs.x = bounds.m_mins.x + (bounds.m_maxs.x - bounds.m_mins.x )* 0.002f;
	borderBounds.m_maxs.y = bounds.m_maxs.y;
	AddVertsForAABB2D(devConsoleQuad, borderBounds, Rgba8::CYAN);

	borderBounds = bounds.GetFractionalAABB2(Vec2(0.998f, 0.0f), Vec2(1.0f, 1.0f));
	AddVertsForAABB2D(devConsoleQuad, borderBounds, Rgba8::CYAN);

	borderBounds = bounds.GetFractionalAABB2(Vec2(0.0f, 0.95f), Vec2(1.0f, 1.0f));
	AddVertsForAABB2D(devConsoleQuad, borderBounds, Rgba8::CYAN);

	std::string header = "THE REMOTE NETWORK CONSOLE : ";
	if (m_server)
	{
		header += "Host";
	}
	else
	{
		header += "Client";
	}
	font->AddVertsForTextInBox2D(devConsoleVerts, borderBounds, 15.0f, header, Rgba8::BLACK,
		0.6f, Vec2(0.0f, 0.0f), TextBoxMode::SHRINK_TO_FIT, 999999, 5.0f);

	borderBounds.m_mins.x = bounds.m_mins.x;
	borderBounds.m_maxs.x = bounds.m_maxs.x;

	borderBounds.m_mins.y = bounds.m_mins.y;
	borderBounds.m_maxs.y = bounds.m_mins.y + (bounds.m_maxs.y- bounds.m_mins.y) * 0.002f;
	AddVertsForAABB2D(devConsoleQuad, borderBounds, Rgba8::CYAN);


	//-------------------------------CONNECTION BOUNDS------------------------------
	textBounds.m_mins.x = connectionBounds.m_mins.x + (connectionBounds.m_maxs.x - connectionBounds.m_mins.x) * 0.02f;
	textBounds.m_mins.y = connectionBounds.m_mins.y + (connectionBounds.m_maxs.y - connectionBounds.m_mins.y) * 0.02f;

	textBounds.m_maxs.y = connectionBounds.m_maxs.y - (connectionBounds.m_maxs.y - connectionBounds.m_mins.y) * 0.02f;
	textBounds.m_maxs.x = connectionBounds.m_maxs.x - (connectionBounds.m_maxs.x - connectionBounds.m_mins.x) * 0.02f;
	for (int i = (int)m_connections.size() - 1; i >= 0; i--)
	{
		std::string address = m_connections[i]->m_address.ToString();
		font->AddVertsForTextInBox2D(devConsoleVerts, textBounds, m_consoleTextHeight, address, Rgba8::WHITE,
			m_consoleTextAspect, Vec2(0.0f, 0.0f), TextBoxMode::SHRINK_TO_FIT, 999999, 5.0f);
		textBounds.m_mins.y += m_consoleTextHeight;
	}

	AABB2 connectionTextBounds;
	connectionTextBounds.m_mins = connectionBounds.m_mins;
	connectionTextBounds.m_maxs.x = connectionBounds.m_mins.x + (connectionBounds.m_maxs.x - connectionBounds.m_mins.x) * 0.002f;
	connectionTextBounds.m_maxs.y = connectionBounds.m_maxs.y;
	AddVertsForAABB2D(devConsoleQuad, connectionTextBounds, Rgba8::GREEN);

	connectionTextBounds = connectionBounds.GetFractionalAABB2(Vec2(0.998f, 0.0f), Vec2(1.0f, 1.0f));
	AddVertsForAABB2D(devConsoleQuad, connectionTextBounds, Rgba8::GREEN);

	connectionTextBounds = connectionBounds.GetFractionalAABB2(Vec2(0.0f, 0.95f), Vec2(1.0f, 1.0f));
	AddVertsForAABB2D(devConsoleQuad, connectionTextBounds, Rgba8::GREEN);

	font->AddVertsForTextInBox2D(devConsoleVerts, connectionTextBounds, 15.0f, "CONNECTED ADDRESSES", Rgba8::BLACK,
		0.6f, Vec2(0.0f, 0.0f), TextBoxMode::SHRINK_TO_FIT, 999999, 5.0f);

	connectionTextBounds.m_mins.x = connectionBounds.m_mins.x;
	connectionTextBounds.m_maxs.x = connectionBounds.m_maxs.x;

	connectionTextBounds.m_mins.y = connectionBounds.m_mins.y;
	connectionTextBounds.m_maxs.y = connectionBounds.m_mins.y + (connectionBounds.m_maxs.y - connectionBounds.m_mins.y) * 0.02f;
	AddVertsForAABB2D(devConsoleQuad, connectionTextBounds, Rgba8::GREEN);

	renderer->DrawVertexArray(int(devConsoleQuad.size()), devConsoleQuad.data());

	if (renderer != nullptr && font != nullptr)
	{
		renderer->BindTexture(&font->GetTexture());
		renderer->DrawVertexArray(int(devConsoleVerts.size()), devConsoleVerts.data());
		renderer->BindTexture(nullptr);
	}
}
std::vector<TCPConnection*> RemoteConsole::GetConnections()
{
	return m_connections;
}
std::vector<NetAddress> NetAddress::GetAllInternal(uint16_t port)
{
	 std::vector<NetAddress> results;
	 char HostName[256];
	 int result =  ::gethostname(HostName, sizeof(HostName));

	 if (result == SOCKET_ERROR)
	 {
		 return results;
	 }
	 addrinfo hints;
	 ::memset(&hints, 0 , sizeof(hints));

	 hints.ai_family = AF_INET;
	 hints.ai_socktype = SOCK_STREAM;

	 addrinfo* addresses = nullptr;
	 int status = ::getaddrinfo(HostName,nullptr, &hints,&addresses);

	 if (status != 0)
	 {
		 return results;
	 }
	 addrinfo* iter = addresses;
	 while (iter != nullptr)
	 {
		NetAddress addr;
		sockaddr_in* ipv4 = (sockaddr_in*) iter->ai_addr;
		addr.address = ::ntohl(ipv4->sin_addr.S_un.S_addr);
		addr.port = port;
		results.push_back(addr);
		iter = iter->ai_next;
	 }
	 ::freeaddrinfo(addresses);
	 return results;
}

std::string NetAddress::ToString()
{
	return Stringf("%u.%u.%u.%u:%u",
		(address & 0xff000000) >> 24,
		(address & 0x00ff0000) >> 16,
		(address & 0x0000ff00) >> 8,
		(address & 0x000000ff) >> 0,
		port
	);
}

NetAddress NetAddress::FromString(std::string const& str)
{
	Strings parts = SplitStringOnDelimiter(str, ':');
	IN_ADDR addr;
	int result = ::inet_pton(AF_INET, parts[0].c_str(), &addr);
	if (result == SOCKET_ERROR)
	{
		return NetAddress();
	}
	uint16_t port = (uint16_t) ::atoi(parts[1].c_str());
	NetAddress address;
	address.address = ::ntohl(addr.S_un.S_addr);
	address.port = port;
	return address;
}

NetAddress NetAddress::GetLoopBack(uint16_t port)
{
	NetAddress addr;
	addr.address = 0x7f000001; // 127 0 0 1
	addr.port = port;
	return addr;
}

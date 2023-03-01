#include "DevConsole.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/VertexUtils.hpp"
#include "Engine/Renderer/DebugRenderer.hpp"
#include "Engine/Core/XmlUtils.hpp"

RemoteConsole* g_remoteConsole = nullptr;
 const Rgba8 DevConsole::Error = Rgba8::RED;
 const Rgba8 DevConsole::Warning = Rgba8(255, 255, 0, 255);
 const Rgba8 DevConsole::INFO_MAJOR = Rgba8(255, 255,255, 255);
 const Rgba8 DevConsole::INFO_MINOR = Rgba8(180, 180, 180, 255);
 const Rgba8 DevConsole::InputTextColor = Rgba8::WHITE;
 const Rgba8 DevConsole::CommandTextColor = Rgba8::GREEN;
 const Rgba8 DevConsole::ConsoleCaretColor = Rgba8::CYAN;

 DevConsole::DevConsole(DevConsoleConfig const& config)
{
	m_config = config;
}

DevConsole::~DevConsole()
{
}

void DevConsole::Startup()
{
	m_caretStopwatch = Stopwatch(&m_clock, m_caretBlinkRate);
	m_lines = std::vector<DevConsoleLine>();
	m_mode = DevConsoleMode::HIDDEN;
	SubscribeBasicEvents();
	SetupRemoteConsole();

}
void DevConsole::SetupRemoteConsole()
{
	RemoteConsoleConfig config;
	config.console = this;
	g_remoteConsole = new RemoteConsole(config);
	g_remoteConsole->Startup();
}
void DevConsole::SubscribeBasicEvents()
{
	g_theEventSystem->SubscribeEventCallbackFunction("HandleCharacterInput", Event_CharInput);
	g_theEventSystem->SubscribeEventCallbackFunction("clear", Command_Clear);
	g_theEventSystem->SubscribeEventCallbackFunction("help", Command_Help);
	g_theEventSystem->SubscribeEventCallbackFunction("HandleEventKeyPressed", Event_KeyPressed);
	g_theEventSystem->SubscribeEventCallbackFunction("Controls", Command_Controls);
	g_theEventSystem->SubscribeEventCallbackFunction("DebugRenderClear", Command_DebugRenderClear);
	//g_theEventSystem->SubscribeEventCallbackFunction("ToggleRaycastDebug", Command_RaycastToggle);
	g_theConsole->AddLine(ConsoleCaretColor, "Type help for a list of commands");
}
void DevConsole::SubscribeRemoteConsoleEvents()
{
	
}
void DevConsole::ShutDown()
{
}

void DevConsole::BeginFrame()
{
	g_remoteConsole->Update();

	if (m_caretStopwatch.CheckDurationElapsedAndDecrement())
	{
		m_caretVisible = !m_caretVisible;
	}
	m_frameNumber++;
}

void DevConsole::EndFrame()
{
}

void DevConsole::Execute(std::string const& consoleCommandText)
{
	EventArgs args;
	if (g_theEventSystem)
	{
		bool meshCommand = MeshLoadingUtilities(consoleCommandText, args);
		bool remoteConsoleCommand = RemoteConsoleUtilities(consoleCommandText, args);
		if (!meshCommand || !remoteConsoleCommand)
		{
			g_theEventSystem->FireEvent(consoleCommandText);
		}

	}
	g_theConsole->m_caretStopwatch.Restart();
}
bool DevConsole::CommandTest(EventArgs& args)
{
	UNUSED((void)args);
	g_theConsole->AddLine(Rgba8::WHITE, Stringf("Starting Game")); // where CONSOLE_INFO is an Rgba8 constant!
	return false; // Does not consume event; continue to call other subscribers’ callback functions
}
bool DevConsole::MeshLoadingUtilities(std::string const& consoleCommandText, EventArgs& args)
{
	int pos = (int)consoleCommandText.find(":");
	std::string modelText = consoleCommandText.substr(0, pos);
	std::string Name = consoleCommandText.substr(pos + 1);
	if (modelText == "ImportModel" || modelText == "LoadModel")
	{
		m_isMeshLoadMode = true;
		args.SetValue("ModelName", Name);
		m_isMeshLoadMode = true;
		g_theEventSystem->FireEvent(modelText, args);
		return true;
	}
	if (m_isMeshLoadMode)
	{
		EventArgs args2;
		args2.SetValue("Options", consoleCommandText);
		g_theEventSystem->FireEvent("MeshOptions", args2);
		g_theEventSystem->FireEvent(consoleCommandText);
		return true;
	}
	return false;
}
bool DevConsole::RemoteConsoleUtilities(std::string const& consoleCommandText, EventArgs& args)
{
	UNUSED((void) args);
	//-----------------RCJOIN------------------------------------------
	if (consoleCommandText.find("RCJoin") != std::string::npos || consoleCommandText.find("rcjoin") != std::string::npos || consoleCommandText.find("RCJOIN") != std::string::npos )
	{
		int addrIndex = (int)consoleCommandText.find("addr");
		if (addrIndex == std::string::npos)
		{
			g_remoteConsole->AddLine(Rgba8::RED, "Failed to specify address. Join Failed");
		}
		/*int port = (consoleCommandText[addrIndex + 5]) - '0';*/
		std::string addressText = consoleCommandText.substr(addrIndex + 5);
		if (addressText == "")
		{ 
			g_remoteConsole->AddLine(Rgba8::RED, "Invalid Address");  
		}
		
		NetAddress addr = NetAddress::FromString(addressText);
		g_remoteConsole->AttemptToJoin(addr);
	}

	//------------------RCHOST----------------------------
	else if (consoleCommandText.find("RCHost") != std::string::npos || consoleCommandText.find("rchost") != std::string::npos || consoleCommandText.find("RCHOST") != std::string::npos)
	{
		int portIndex = (int)consoleCommandText.find("port");
		int port = std::stoi(consoleCommandText.substr(portIndex+5));
		if (portIndex != std::string::npos)
		{
			g_remoteConsole->AttemptToHost((uint16_t)port);
		}
		else
		{
			g_remoteConsole->AttemptToHost(3121);
		}
	}

	//----------------RCA ALL COMMANDS----------------------------------------------
	else if ((consoleCommandText[0] == 'R' || consoleCommandText[0] == 'r') && (consoleCommandText[1] == 'C' || consoleCommandText[1] == 'c') && (consoleCommandText[2] == 'A' || consoleCommandText[2] == 'a'))
	{
		int cmdInt = (int)consoleCommandText.find("cmd");
		if (cmdInt != std::string::npos)
		{
			for (int i = 0; i < g_remoteConsole->GetConnections().size(); i++)
			{
				std::string sendstring = consoleCommandText.substr(cmdInt + 4);
				g_remoteConsole->SendCommand(i, sendstring, false);
			}
		}
	}
	//---------------------RCLEAVE--------------------------------------------
	else if (consoleCommandText.find("RCLeave") != std::string::npos || consoleCommandText.find("rcleave") != std::string::npos || consoleCommandText.find("RCLEAVE") != std::string::npos)
	{
		g_remoteConsole->DeleteConnections();
		g_remoteConsole->AddLine(Rgba8::GREEN, "Deleted all connections and Left successfully");
	}

	//---------------------RCKICK--------------------------------------------
	else if (consoleCommandText.find("RCKick") != std::string::npos || consoleCommandText.find("rckick") != std::string::npos || consoleCommandText.find("RCKICK") != std::string::npos)
	{
		int idxInt = (int)consoleCommandText.find("idx");
		int index = (consoleCommandText[idxInt + 4]) - '0';
		if (index != std::string::npos)
		{ 
			g_remoteConsole->KickPlayerAtIndex(index);
		}

	}

	//-----------------RCECHO------------------------------
	else if (consoleCommandText.find("RCEcho") != std::string::npos || consoleCommandText.find("rcecho") != std::string::npos || consoleCommandText.find("RCECHO") != std::string::npos)
	{
		int idxInt = (int)consoleCommandText.find("idx");
		int msgInt = (int)consoleCommandText.find("msg");
		int index = (consoleCommandText[idxInt + 4]) - '0';
		if (idxInt == std::string::npos) { index = 0; }
		if (msgInt != std::string::npos)
		{
			std::string sendstring = consoleCommandText.substr(msgInt + 4);
			g_remoteConsole->SendCommand(index, sendstring, true);
		}

	}
	//-------------------RC OTHER COMMANDS-----------------------------------
	else if ((consoleCommandText[0] == 'R' || consoleCommandText[0] == 'r') && (consoleCommandText[1] == 'C' || consoleCommandText[1] == 'c'))
	{
		int idxInt = (int)consoleCommandText.find("idx");
		int cmdInt = (int)consoleCommandText.find("cmd");
		int index = (consoleCommandText[idxInt + 4]) - '0';
		if (idxInt == std::string::npos) { idxInt = 0; }
		if (cmdInt != std::string::npos)
		{
			std::string sendstring = consoleCommandText.substr(cmdInt + 4);
			g_remoteConsole->SendCommand(index, sendstring , false);
		}
	}

	return false;
}
void DevConsole::AddLine(Rgba8 const& color, std::string const& text)
{
	DevConsoleLine line;
	line.text = text;
	line.color = color;
	line.frame = m_frameNumber;
	line.timeStamp = GetCurrentTimeSeconds();
	m_lines.push_back(line);
}

void DevConsole::Render(AABB2 const& bounds, Renderer* rendererOverride) const
{
	m_config.m_renderer->BindTexture(nullptr);
	m_config.m_renderer->BindShader(m_config.m_renderer->GetDefaultShader());
	m_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
	m_config.m_renderer->SetDepthStencilState(DepthTest::ALWAYS, false);
	m_config.m_renderer->SetRasterizerState(CullMode::NONE, FillMode::SOLID, WindingOrder::COUNTERCLOCKWISE);
	m_config.m_renderer->SetSamplerMode(SamplerMode::POINTCLAMP);
	std::vector<Vertex_PCU> devConsoleVerts;
	std::vector<Vertex_PCU> devConsoleQuad;
	BitmapFont* font = m_config.m_renderer->CreateOrGetBitmapFont("Data/Images/SquirrelFixedFont");


	AABB2 textBounds = bounds.GetFractionalAABB2(Vec2(0.002f, 0.005f), Vec2(0.998f, 0.995f));

	AddVertsForAABB2D(devConsoleQuad, bounds, Rgba8(0, 0, 0, 200));
	AABB2 devConsoleInputBounds;

	devConsoleInputBounds.m_mins.x = bounds.m_maxs.x * 0.002f;
	devConsoleInputBounds.m_mins.y = bounds.m_maxs.y * 0.005f;
	devConsoleInputBounds.m_maxs.x = bounds.m_maxs.x * 0.998f;
	devConsoleInputBounds.m_maxs.y = bounds.m_mins.y + m_consoleTextHeight + 2.0f;
	AddVertsForAABB2D(devConsoleQuad, devConsoleInputBounds, Rgba8(150, 150, 150, 100));
	
	font->AddVertsForTextInBox2D(devConsoleVerts, textBounds, m_consoleTextHeight, m_inputText, InputTextColor,
		m_consoleTextAspect, Vec2(0.0f, 0.0f), TextBoxMode::SHRINK_TO_FIT, 999999, 5.0f);

	if (m_caretVisible)
	{
		AABB2 caretBounds;
		caretBounds.m_mins.x = devConsoleInputBounds.m_mins.x + (float)(m_caretPosition * m_consoleTextHeight * m_consoleTextAspect);
		caretBounds.m_mins.y = devConsoleInputBounds.m_mins.y;
		caretBounds.m_maxs.x = caretBounds.m_mins.x + (m_consoleTextHeight * m_consoleTextAspect * 0.2f);
		caretBounds.m_maxs.y = m_consoleTextHeight;
		AddVertsForAABB2D(devConsoleQuad, caretBounds, ConsoleCaretColor);
	}

	for (int i = (int)m_lines.size() - 1; i >= 0; i--)
	{
		textBounds.m_mins.y += m_consoleTextHeight;
		font->AddVertsForTextInBox2D(devConsoleVerts, textBounds, m_consoleTextHeight, m_lines[i].text, m_lines[i].color,
			m_consoleTextAspect, Vec2(0.0f, 0.0f), TextBoxMode::SHRINK_TO_FIT, 999999, 5.0f);
	}

	//-----------------------------DEV CONSOLE BORDERS-------------------------------------------
	AABB2 borderBounds;
	borderBounds.m_mins = Vec2(0.0f, 0.0f);
	borderBounds.m_maxs.x = bounds.m_maxs.x * 0.002f;
	borderBounds.m_maxs.y = bounds.m_maxs.y;
	AddVertsForAABB2D(devConsoleQuad, borderBounds, Rgba8::CYAN);

	borderBounds = bounds.GetFractionalAABB2(Vec2(0.0f, 0.995f), Vec2(1.0f, 1.0f));
	AddVertsForAABB2D(devConsoleQuad, borderBounds, Rgba8::CYAN);

	borderBounds = bounds.GetFractionalAABB2(Vec2(0.998f, 0.0f), Vec2(1.0f, 1.0f));
	AddVertsForAABB2D(devConsoleQuad, borderBounds, Rgba8::CYAN);

	borderBounds = bounds.GetFractionalAABB2(Vec2(0.0f, 0.984f), Vec2(1.0f, 1.0f));
	AddVertsForAABB2D(devConsoleQuad, borderBounds, Rgba8::CYAN);

	

	font->AddVertsForTextInBox2D(devConsoleVerts, borderBounds, 15.0f, "THE DEVELOPER CONSOLE", Rgba8::BLACK,
		0.6f, Vec2(0.0f, 0.0f), TextBoxMode::SHRINK_TO_FIT, 999999, 5.0f);

	borderBounds.m_mins.x = bounds.m_maxs.x * 0.002f;
	borderBounds.m_maxs.x = bounds.m_maxs.x * 0.998f;

	borderBounds.m_mins.y = bounds.m_mins.y;
	borderBounds.m_maxs.y = bounds.m_maxs.y * 0.005f;
	AddVertsForAABB2D(devConsoleQuad, borderBounds, Rgba8::CYAN);

	m_config.m_renderer->DrawVertexArray(int(devConsoleQuad.size()), devConsoleQuad.data());

	if (rendererOverride != nullptr && font != nullptr)
	{
		rendererOverride->BindTexture(&font->GetTexture());
		rendererOverride->DrawVertexArray(int(devConsoleVerts.size()), devConsoleVerts.data());
		rendererOverride->BindTexture(nullptr);
	}
	else
	{
		m_config.m_renderer->BindTexture(&font->GetTexture());
		m_config.m_renderer->DrawVertexArray(int(devConsoleVerts.size()), devConsoleVerts.data());
		m_config.m_renderer->BindTexture(nullptr);
	}

	g_remoteConsole->Render(bounds, m_config.m_renderer);
	font = nullptr;
	devConsoleVerts.clear();
	devConsoleQuad.clear();
}

void DevConsole::RegisterCommands(std::string commandText)
{
	if (m_historyIndex < m_maxCommandHistory)
	{
		m_historyIndex = m_historyIndex+1;
		m_currentCommandHistoryIndexSelected = m_historyIndex;
		m_commandHistory.push_back(commandText);
	}
	else if (m_historyIndex >= m_maxCommandHistory)
	{
		m_historyIndex = 0;
		m_commandHistory[m_historyIndex] = commandText;
	}
	g_theConsole->AddLine(CommandTextColor, commandText);
	//g_theConsole->m_caretStopwatch.Restart();
}

DevConsoleMode DevConsole::GetMode() const
{
	return m_mode;
}

void DevConsole::SetMode(DevConsoleMode mode)
{
	m_mode = mode;
}

void DevConsole::ToggleMode(DevConsoleMode mode)
{
	UNUSED((void)mode);
	if (m_mode == DevConsoleMode::HIDDEN)
	{
		m_mode = DevConsoleMode::VISIBLE;
	}
	else
	{
		m_mode = DevConsoleMode::HIDDEN;
	}
}

bool DevConsole::Event_KeyPressed(EventArgs& args)
{
	unsigned char keyValue = args.GetValue("KeyName", (unsigned char)0);
	if (keyValue == LEFT_ARROW)
	{
		if (g_theConsole->m_caretPosition > 0)
		{
			g_theConsole->m_caretPosition--;
		}
	
	}
	if (keyValue == RIGHT_ARROW)
	{
		if (g_theConsole->m_caretPosition < g_theConsole->m_inputText.size())
		{
			g_theConsole->m_caretPosition++;
		}
	}
	if (keyValue == UP_ARROW)
	{
		if (g_theConsole->m_currentCommandHistoryIndexSelected <= g_theConsole->m_historyIndex 
			&& g_theConsole->m_currentCommandHistoryIndexSelected >= 0 )
		{
			g_theConsole->m_inputText.clear();
			g_theConsole->m_inputText = g_theConsole->m_commandHistory[g_theConsole->m_currentCommandHistoryIndexSelected];
			g_theConsole->m_caretPosition = (int)g_theConsole->m_inputText.size();
			if(g_theConsole->m_currentCommandHistoryIndexSelected>0)
				g_theConsole->m_currentCommandHistoryIndexSelected--;
		}
	}
	if (keyValue == DOWN_ARROW)
	{
		if (g_theConsole->m_currentCommandHistoryIndexSelected <= g_theConsole->m_historyIndex
			&& g_theConsole->m_currentCommandHistoryIndexSelected <= g_theConsole->m_maxCommandHistory
			&& g_theConsole->m_currentCommandHistoryIndexSelected >= 0)
		{
			if (g_theConsole->m_currentCommandHistoryIndexSelected == 0)
			{
				g_theConsole->m_currentCommandHistoryIndexSelected++;
			}
			if (!g_theConsole->m_inputText.empty())
			{
				g_theConsole->m_inputText.clear();
				g_theConsole->m_inputText = g_theConsole->m_commandHistory[g_theConsole->m_currentCommandHistoryIndexSelected];
				g_theConsole->m_caretPosition = (int)g_theConsole->m_inputText.size();
			}
			else
			{
				g_theConsole->m_currentCommandHistoryIndexSelected--;
			}
			if (g_theConsole->m_currentCommandHistoryIndexSelected != 0)
			{
				g_theConsole->m_currentCommandHistoryIndexSelected++;
			}
		}
		else if (g_theConsole->m_currentCommandHistoryIndexSelected >= g_theConsole->m_commandHistory.size())
		{
			g_theConsole->m_currentCommandHistoryIndexSelected = g_theConsole->m_historyIndex;
			g_theConsole->m_inputText.clear();
			g_theConsole->m_caretPosition = 0;
		}
	}
	if (keyValue == ENTER_KEY)
	{
		g_theConsole->RegisterCommands(g_theConsole->m_inputText);
		g_theConsole->Execute(g_theConsole->m_inputText);
		g_theConsole->m_inputText.clear();
		g_theConsole->m_caretPosition = 0;
	}
	if (keyValue == BACKSPACE_KEY)
	{
		if (g_theConsole->m_caretPosition > 0)
		{
			g_theConsole->m_inputText.erase(g_theConsole->m_caretPosition - 1, 1);
			g_theConsole->m_caretPosition--;
		}
	}
	if (keyValue == DELETE_KEY)
	{
		g_theConsole->m_inputText.erase(g_theConsole->m_caretPosition, 1);
	}
	if (keyValue == END_KEY)
	{
		g_theConsole->m_caretPosition = (int)g_theConsole->m_inputText.size();
	}
	if (keyValue == HOME_KEY)
	{
		g_theConsole->m_caretPosition = 0;
	}
	return true;
}

bool DevConsole::Event_CharInput(EventArgs& args)
{
	unsigned char charCode = (char)args.GetValue("KeyName", 0);
	if ((charCode >= 32) && (charCode <= 126) && (charCode != '`') && (charCode != '~') && (charCode != '"') && g_theConsole->m_mode == DevConsoleMode::VISIBLE)
	{
		g_theConsole->m_inputText.insert(g_theConsole->m_caretPosition, 1, charCode);
		g_theConsole->m_caretPosition++;
	}
	return true;
}

bool DevConsole::Command_Clear(EventArgs& args)
{
	UNUSED((void)args);
	if (g_theConsole)
	{
		g_theConsole->m_lines.clear();
		g_theConsole->m_commandHistory.clear();
		g_theConsole->m_historyIndex = -1;
		g_theConsole->m_currentCommandHistoryIndexSelected = -1;
	}
	return false;
}

bool DevConsole::Command_Help(EventArgs& args)
{
	UNUSED((void)args);
	std::vector<std::string> commandText; 
	g_theEventSystem->GetRegisteredEventNames(commandText);
	for (int i = (int)commandText.size() - 1; i >= 0; i--)
	{
		g_theConsole->RegisterCommands(commandText[i]);
	}
	g_theConsole->RegisterCommands("RCJoin");
	g_theConsole->RegisterCommands("RCHost");
	g_theConsole->RegisterCommands("RCLeave");
	g_theConsole->RegisterCommands("RCKick");
	g_theConsole->RegisterCommands("RCA cmd=");
	g_theConsole->RegisterCommands("RCEcho idx= msg=");
	g_theConsole->RegisterCommands("RC idx= cmd=");
	return false;
}

bool DevConsole::Command_Controls(EventArgs& args)
{
	UNUSED((void)args);
	if (g_theConsole)
	{
		g_theConsole->AddLine(InputTextColor, "Game Controls");
		g_theConsole->AddLine(InputTextColor, "Shift          - Increase Speed");
		g_theConsole->AddLine(InputTextColor, "W / S          - Move Forward/Backwad");
		g_theConsole->AddLine(InputTextColor, "A / D          - Move Left/Right");
		g_theConsole->AddLine(InputTextColor, "Z / C          - Move Up/Down");
		g_theConsole->AddLine(InputTextColor, "Mouse controls - Rotate Player");
		g_theConsole->AddLine(InputTextColor, "Q/E            - Roll Player");
		g_theConsole->AddLine(InputTextColor, "H              - Reset Player");
		g_theConsole->AddLine(InputTextColor, "Debug Controls");
		g_theConsole->AddLine(InputTextColor, "1              - Spawn Debug Sphere");
		g_theConsole->AddLine(InputTextColor, "2              - Spawn Debug Line");
		g_theConsole->AddLine(InputTextColor, "3              - Spawn Camera Basis");
		g_theConsole->AddLine(InputTextColor, "4              - Spawn Billboard text");
		g_theConsole->AddLine(InputTextColor, "5              - Spawn Cylinder");
		g_theConsole->AddLine(InputTextColor, "6              - Add Screen message for camera orientation");
		g_theConsole->AddLine(InputTextColor, "9              - Slow Debug Clock");
		g_theConsole->AddLine(InputTextColor, "0              - Speed up Debug Clock");
	}
	return false;
}

bool DevConsole::Command_DebugRenderClear(EventArgs& args)
{
	UNUSED((void)args);
	DebugRenderClear();
	return false;
}

void DevConsole::Render_OpenFull(AABB2 const& bounds, Renderer& renderer, BitmapFont& font, float fontAspect) const
{
	UNUSED((void)bounds);
	UNUSED((void)renderer);
	UNUSED((void)font);
	UNUSED((void)fontAspect);
}

void DevConsole::ExecuteXmlCommandScriptNode(tinyxml2::XMLElement  const& commandScriptXmlElement)
{
	const XmlElement* element = commandScriptXmlElement.FirstChildElement();
	while (element != nullptr)
	{
		std::string functionName = element->Name();
		const XmlAttribute* attributeElements = element->FirstAttribute();
		EventArgs args;
		while (attributeElements != nullptr)
		{
			std::string keyName = attributeElements->Name();
			std::string keyValue = attributeElements->Value();
			args.SetValue(keyName, keyValue);
			attributeElements = attributeElements->Next();
		}
		g_theEventSystem->FireEvent(functionName, args);
		element = element->NextSiblingElement();
	}
	
}

void DevConsole::ExecuteXmlCommandScriptFile(std::string& filePath)
{
	tinyxml2::XMLDocument xml_doc;
	tinyxml2::XMLError result = xml_doc.LoadFile(filePath.c_str());
	if (result != tinyxml2::XML_SUCCESS)
		return;

	tinyxml2::XMLElement* rootElement = xml_doc.RootElement();
	if (rootElement != nullptr)
	{
		ExecuteXmlCommandScriptNode(*rootElement);
	}

}
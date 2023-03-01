#pragma once
#include <string>
#include "Engine/Core/Rgba8.hpp"
#include <vector>
#include "Engine/Core/Time.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Stopwatch.hpp"
#include "Engine/Networking/Net.hpp"
#include "Engine/Core/XmlUtils.hpp"
#include <mutex>
class Renderer;
struct AABB2;
class BitmapFont;

struct DevConsoleConfig
{
	Renderer* m_renderer;
};
enum class DevConsoleMode
{
	HIDDEN,
	VISIBLE,
	TOTAL
};
struct DevConsoleLine
{
	std::string text;
	Rgba8 color;
	int frame;
	double timeStamp;
};
enum RaycastToggleMode
{
	RaycastVisible,
	RaycastHidden
};
class DevConsole
{
	public:
		DevConsole(DevConsoleConfig const& config);
		~DevConsole();
		void Startup();
		void ShutDown();
		void BeginFrame();
		void EndFrame();

		void SetupRemoteConsole();
		void SubscribeBasicEvents();
		void SubscribeRemoteConsoleEvents();
		void Execute(std::string const& consoleCommandText);
		static bool CommandTest(EventArgs& args);
		bool MeshLoadingUtilities(std::string const& consoleCommandText, EventArgs& args);
		bool RemoteConsoleUtilities(std::string const& consoleCommandText, EventArgs& args);
		void AddLine(Rgba8 const& color, std::string const& text);
		void Render(AABB2 const& bounds, Renderer* rendererOverride = nullptr) const;
		void RegisterCommands(std::string commandText);

		DevConsoleMode GetMode() const;
		void SetMode(DevConsoleMode mode);
		void ToggleMode(DevConsoleMode mode);
		void ExecuteXmlCommandScriptNode(tinyxml2::XMLElement const& commandScriptXmlElement);
		void ExecuteXmlCommandScriptFile(std::string& filePath);

		static bool Event_KeyPressed(EventArgs& args);
		static bool Event_CharInput(EventArgs& args);
		static bool Command_Clear(EventArgs& args);
		static bool Command_Help(EventArgs& args);
		static bool	Command_Controls(EventArgs& args);
		static bool	Command_DebugRenderClear(EventArgs& args);


		const static  Rgba8 Error;
		const static  Rgba8 Warning;
		const static  Rgba8 INFO_MAJOR;
		const static  Rgba8 INFO_MINOR;
		const static  Rgba8 InputTextColor;
		const static  Rgba8 ConsoleCaretColor;
		const static  Rgba8 CommandTextColor;
		bool							m_isMeshLoadMode = false;
		DevConsoleMode					m_mode = DevConsoleMode::HIDDEN;
		Clock							m_clock;
		RaycastToggleMode				m_raycastMode = RaycastToggleMode::RaycastVisible;
		std::vector<DevConsoleLine>		m_lines;
protected:
	void Render_OpenFull(AABB2 const& bounds, Renderer& renderer, BitmapFont& font, float fontAspect = 1.0f) const;

protected:
	float							m_consoleTextHeight = 20.0f;
	float							m_consoleTextAspect = 0.6f;
	int								m_frameNumber = 0;
	DevConsoleConfig				m_config;

	std::mutex						m_lineMutex;
	
	Stopwatch						m_caretStopwatch;
	std::string						m_inputText;
	int								m_caretPosition = 0;
	double							m_caretBlinkRate = 0.5;
	bool							m_caretVisible = true;
	int								m_maxCommandHistory = 128;
	std::vector< std::string >		m_commandHistory;
	int								m_historyIndex = -1;
	int								m_currentCommandHistoryIndexSelected = -1;
	
};


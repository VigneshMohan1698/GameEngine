#pragma once
#include <string>
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/Vertex_PNCU.hpp"
#include "Engine/Core/Vertex_PNCUTB.hpp"
#include "Engine/Core/XmlUtils.hpp"

//#include "Engine/Core/DevConsole.hpp"
//#pragma warning(error:4172) // Gives me an error for warnings;
#define  UNUSED(x) void(x);

extern NamedStrings g_gameConfigBlackboard;
extern EventSystem* g_theEventSystem;
extern DevConsole* g_theConsole;

typedef tinyxml2::XMLElement XmlElement;
typedef tinyxml2::XMLAttribute XmlAttribute;
static constexpr int UP_ARROW =38;
static constexpr int LEFT_ARROW =37;
static constexpr int DOWN_ARROW =40;
static constexpr int RIGHT_ARROW =39;
static constexpr int ENTER_KEY =13;
static constexpr int ESCAPE_KEY =27;
static constexpr int HOME_KEY =36;
static constexpr int END_KEY =35;
static constexpr int DELETE_KEY=46;
static constexpr int BACKSPACE_KEY =8;

void PopulateGameConfigs(std::string XMLfilePath);
std::wstring ConvertToWString(const char* c);
typedef std::vector<Vertex_PCU> VertexArray;
typedef std::vector<Vertex_PNCU> VertexNormalArray;
typedef std::vector<Vertex_PNCUTB> VertexNormalTangentArray;
typedef std::vector<unsigned int> IndexArray;

//void* operator new(size_t bytes);
//void operator delete(void* ptr);
void OutputMemoryLeaks();
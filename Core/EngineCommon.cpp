#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EventSystem.hpp"
#include <Engine/Core/ErrorWarningAssert.hpp>

NamedStrings g_gameConfigBlackboard;

int g_numMemoryAllocated = 0;
void PopulateGameConfigs(const char* XMLfilePath)
{
//	tinyxml2::XMLDocument xml_doc;
//
//	tinyxml2::XMLError result = xml_doc.LoadFile(XMLfilePath);
//	if (result != tinyxml2::XML_SUCCESS)
//		return;
//
//	tinyxml2::XMLElement* element = xml_doc.FirstChildElement("GameConfig");
//
//	if (element == nullptr) 
//		return;
//
//	if (element != nullptr)
//	{
//		g_gameConfigBlackboard.PopulateFromXmlElementAttributes(*element);
//	}
//	return;

}


std::wstring ConvertToWString(const char* c)
{
	std::string s {c};
	return {s.begin(), s.end()};
}
//
//void* operator new(size_t bytes)
//{
//	++g_numMemoryAllocated;
//	return ::malloc(bytes);
//}
//
//void operator delete(void* ptr)
//{
//	::free(ptr);
//	g_numMemoryAllocated--;
//}

void EngineShutdown()
{
	std::string text = "Memory leaks : " + g_numMemoryAllocated;
 	//DebuggerPrintf(text.c_str());
}


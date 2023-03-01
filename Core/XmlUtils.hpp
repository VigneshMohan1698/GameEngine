#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "ThirdParty/TinyXML2/tinyxml2.h"
#include <string>
#include "Engine/Math/FloatRange.hpp"

int ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, int defaultValue);
char ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, char defaultValue);
bool ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, bool defaultValue);
float ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, float defaultValue);
Rgba8 ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, Rgba8 const& defaultValue);
Vec2 ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, Vec2 const& defaultValue);
Vec3 ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, Vec3 const& defaultValue);
IntVec2 ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, IntVec2 const& defaultValue);
std::string ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, std::string const& defaultValue);
Strings ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, Strings const& defaultValues);
FloatRange ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, FloatRange const& defaultValues);



#include "NamedStrings.hpp"
#include "Engine/Core/StringUtils.hpp"

 void NamedStrings::PopulateFromXmlElementAttributes(tinyxml2::XMLElement const& element)
{
	for (const tinyxml2::XMLAttribute* attributes = element.FirstAttribute(); attributes; attributes = attributes->Next())
	{
		m_keyValuePairs.insert({ attributes->Name(),attributes->Value() });
	}
}

void NamedStrings::SetValue(std::string const& keyName, std::string const& newValue)
{
	m_keyValuePairs[keyName] = newValue;
}

std::string NamedStrings::GetValue(std::string const& keyName, std::string const& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator it = m_keyValuePairs.find(keyName);
	if (it != m_keyValuePairs.end())
	{
		return m_keyValuePairs.find(keyName)->second;
	}
	else
	{
		return defaultValue;
	}
}

bool NamedStrings::GetValue(std::string const& keyName, bool defaultValue) const
{
	std::map<std::string, std::string>::const_iterator it = m_keyValuePairs.find(keyName);
	if (it != m_keyValuePairs.end())
	{
		if (m_keyValuePairs.find(keyName)->second == "true" || m_keyValuePairs.find(keyName)->second == "1" || m_keyValuePairs.find(keyName)->second == "True" || m_keyValuePairs.find(keyName)->second == "TRUE")
			return true;
		else if (m_keyValuePairs.find(keyName)->second == "false" || m_keyValuePairs.find(keyName)->second == "0" || m_keyValuePairs.find(keyName)->second == "False" || m_keyValuePairs.find(keyName)->second == "FALSE")
			return false;
		else
			return false;
	}
	else
	{
		return defaultValue;
	}
}

int NamedStrings::GetValue(std::string const& keyName, int defaultValue) const
{
	std::map<std::string, std::string>::const_iterator it = m_keyValuePairs.find(keyName);
	if (it != m_keyValuePairs.end())
	{
		return std::stoi(m_keyValuePairs.find(keyName)->second);
	}
	else
	{
		return defaultValue;
	}
}

float NamedStrings::GetValue(std::string const& keyName, float defaultValue) const
{
	std::map<std::string, std::string>::const_iterator it = m_keyValuePairs.find(keyName);
	if (it != m_keyValuePairs.end())
	{
		return std::stof(m_keyValuePairs.find(keyName)->second);
	}
	else
	{
		return defaultValue;
	}
}

std::string NamedStrings::GetValue(std::string const& keyName, char const* defaultValue) const
{
	std::map<std::string, std::string>::const_iterator it = m_keyValuePairs.find(keyName);
	if (it != m_keyValuePairs.end())
	{
		return m_keyValuePairs.find(keyName)->second;
	}
	else
	{
		return defaultValue;
	}
}

Rgba8 NamedStrings::GetValue(std::string const& keyName, Rgba8 const& defaultValue) const
{
	Rgba8 returnValue;
	std::map<std::string, std::string>::const_iterator it = m_keyValuePairs.find(keyName);
	if (it != m_keyValuePairs.end())
	{
		returnValue.SetFromText(m_keyValuePairs.find(keyName)->second.c_str());
		return returnValue;
	}
	else
	{
		return defaultValue;
	}
}

Vec2 NamedStrings::GetValue(std::string const& keyName, Vec2 const& defaultValue) const
{
	Vec2 returnValue;
	std::map<std::string, std::string>::const_iterator it = m_keyValuePairs.find(keyName);
	if (it != m_keyValuePairs.end())
	{
		returnValue.SetFromText(m_keyValuePairs.find(keyName)->second.c_str());
		return returnValue;
	}
	else
	{
		return defaultValue;
	}
}

IntVec2 NamedStrings::GetValue(std::string const& keyName, IntVec2 const& defaultValue) const
{
	IntVec2 returnValue;
	std::map<std::string, std::string>::const_iterator it = m_keyValuePairs.find(keyName);
	if (it != m_keyValuePairs.end())
	{
		returnValue.SetFromText(m_keyValuePairs.find(keyName)->second.c_str());
		return returnValue;
	}
	else
	{
		return defaultValue;
	}
}

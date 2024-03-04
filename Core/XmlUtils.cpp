
#include "XmlUtils.hpp"

int ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, int defaultValue)
{
	if (element.Attribute(attributeName))
	{
		std::string attributeValue = element.Attribute(attributeName);
		if (!attributeValue.empty())
		{
			return std::stoi(attributeValue);
		}
		else
		{
			return defaultValue;
		}
	}
	else return defaultValue;
}

char ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, char defaultValue)
{
	std::string attributeValueString = element.Attribute(attributeName);
	if (!attributeValueString.empty())
	{
		const char* attributeValue = element.Attribute(attributeName);
		return *attributeValue;
	}
	else
	{
		return defaultValue;
	}
	
}

bool ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, bool defaultValue)
{
	if (!element.Attribute(attributeName))
	{
		return defaultValue;
	}
	std::string attributeValue = element.Attribute(attributeName);
	if (!attributeValue.empty())
	{
		if (attributeValue == "true" || attributeValue == "1" || attributeValue == "True" || attributeValue == "TRUE")
			return true;
		else if (attributeValue == "false" || attributeValue == "0" || attributeValue == "False" || attributeValue == "FALSE")
			return false;
		else
			return false;
	}
	else
	{
		return defaultValue;
	}
}

float ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, float defaultValue)
{
	if (!element.Attribute(attributeName))
	{
		return defaultValue;
	}
	std::string attributeValue = element.Attribute(attributeName);
	if (!attributeValue.empty())
	{
		return std::stof(attributeValue);
	}
	else
	{
		return defaultValue;
	}
}

Rgba8 ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, Rgba8 const& defaultValue)
{
	Rgba8 returnValue = Rgba8::WHITE;
	if (element.Attribute(attributeName))
	{
		const char* attributeValue = element.Attribute(attributeName);
		if (attributeValue != nullptr)
		{
			returnValue.SetFromText(attributeValue);
			return returnValue;
		}
		else
		{
			return defaultValue;
		}
	}
	else
	{
		return defaultValue;
	}
	
}

Vec2 ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, Vec2 const& defaultValue)
{
	Vec2 returnValue;
	const char* attributeValue = element.Attribute(attributeName);
	if (attributeValue != nullptr)
	{
		returnValue.SetFromText(attributeValue);
		return returnValue;
	}
	else
	{
		return defaultValue;
	}
}

Vec3 ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, Vec3 const& defaultValue)
 {
	Vec3 returnValue;
	const char* attributeValue = element.Attribute(attributeName);
	if (attributeValue != nullptr)
	{
		returnValue.SetFromText(attributeValue);
		return returnValue;
	}
	else
	{
		return defaultValue;
	}
}

IntVec2 ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, IntVec2 const& defaultValue)
{
	IntVec2 returnValue;
	if (element.Attribute(attributeName))
	{
		const char* attributeValue = element.Attribute(attributeName);
		if (attributeValue != nullptr)
		{
			returnValue.SetFromText(attributeValue);
			return returnValue;
		}
		else
		{
			return defaultValue;
		}
	}
	else
	{
		return defaultValue;
	}
	
}

std::string ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, std::string const& defaultValue)
{
	if (element.Attribute(attributeName))
	{
		std::string attributeValue = "";
		if (element.Attribute(attributeName))
		{
			attributeValue = element.Attribute(attributeName);
		}

		if (!attributeValue.empty())
		{
			return attributeValue;
		}
		else
		{
			return defaultValue;
		}
	}
	else return defaultValue;
	
}

Strings ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, Strings const& defaultValues)
{
	std::string attributeValue = element.Attribute(attributeName);
	if (!attributeValue.empty())
	{
		return SplitStringOnDelimiter(attributeValue,',');
	}
	else
	{
		return defaultValues;
	}
}

FloatRange ParseXmlAttribute(tinyxml2::XMLElement const& element, char const* attributeName, FloatRange const& defaultValues)
{
	FloatRange returnValue;
	if (element.Attribute(attributeName))
	{
		const char* attributeValue = element.Attribute(attributeName);
		if (attributeValue != nullptr)
		{
			returnValue.SetFromText(attributeValue);
			return returnValue;
		}
		else
		{
			return defaultValues;
		}
	}
	else
	{
		return defaultValues;
	}
}


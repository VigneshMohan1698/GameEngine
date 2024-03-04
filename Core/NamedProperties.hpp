#pragma once
#include <string>
#include "Engine/Core/Rgba8.hpp"
#include <map>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/IntVec2.hpp>
#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

class PropertyBase
{	
	friend class NamedProperties;
	public:
	PropertyBase();
	virtual ~PropertyBase();

};

template <typename T>
class TypedProperty : public PropertyBase
{
	friend class NamedProperties;
	public:
	TypedProperty(const T& value) {m_value = value;};
	public:
	T m_value;
	//svirtual ~TypedProperty();


};


class NamedProperties
{
public:
	/*void									PopulateFromXmlElementAttributes(tinyxml2::XMLElement const& element);*/
	template <typename T>
	void									SetValue(std::string keyName, T const& setValue) ;
	template <typename T>
	T										GetValue(std::string keyName, T const& defaultvalue) const;
	/*~NamedProperties() 
	{
		for (std::map<std::string, PropertyBase*>::iterator it = m_properties.begin(); it != m_properties.end(); it++)
		{
			delete it->second;
		}
	};*/
private:
	std::map< std::string, PropertyBase*  >	m_properties;

};

template<typename T>
inline T NamedProperties::GetValue(std::string keyName, T const& defaultvalue) const 
{
	std::map<std::string, PropertyBase*>::const_iterator found = m_properties.find(keyName);
	if (found == m_properties.end())
	{
		return defaultvalue;
	}
	TypedProperty<T>* sameTypeProperty = dynamic_cast<TypedProperty<T>*>(found->second);
	if (sameTypeProperty)
	{
		return sameTypeProperty->m_value;
	}
	//ERROR_AND_DIE("Named Properties looking for wrong type");

}

template<typename T>
void NamedProperties::SetValue(std::string keyName, T const& setValue)
{
	for (auto property : m_properties)
	{
		if (property.first == keyName)
		{
			TypedProperty<T>* possibleMatch = dynamic_cast<TypedProperty<T>*>(property.second);
			if (possibleMatch)
			{
				possibleMatch->m_value = setValue;
				return;
			}
			else
			{
				delete property.second;
				property.second = new TypedProperty<T>(setValue);
				return;
			}
		}
	}
	TypedProperty<T>* property = new TypedProperty<T>(setValue);
	m_properties.insert({ keyName , property });
}


class HashedCaseInsensitiveString
{
	public:
	HashedCaseInsensitiveString(std::string value) 
	{
		m_textCaseIntact  = value;
		m_lowerCaseHash = CalculateHash(value);
	}
	std::string m_textCaseIntact;
	unsigned int m_lowerCaseHash = 0;
	bool operator==(HashedCaseInsensitiveString const& compare)
	{
		if (m_lowerCaseHash != compare.m_lowerCaseHash)
		{
			return false;
		}
		else
		{
            #ifndef __APPLE__
			return _stricmp(m_textCaseIntact.c_str(), compare.m_textCaseIntact.c_str()) == 0;
            #else
            return strcmp(m_textCaseIntact.c_str(), compare.m_textCaseIntact.c_str()) == 0;
            #endif
		}
	}
	bool operator!=(HashedCaseInsensitiveString const& compare)
	{
		if (m_lowerCaseHash != compare.m_lowerCaseHash)
		{
			return true;
		}
		else
		{
            #ifndef __APPLE__
            return !_stricmp(m_textCaseIntact.c_str(), compare.m_textCaseIntact.c_str()) == 0;
            #else
            return strcmp(m_textCaseIntact.c_str(), compare.m_textCaseIntact.c_str()) == 0;
            #endif
		}
	}

	bool operator==(std::string  compareString)
	{
		HashedCaseInsensitiveString compare(compareString);
		if (m_lowerCaseHash != compare.m_lowerCaseHash)
		{
			return false;
		}
		else
		{
            #ifndef __APPLE__
            return _stricmp(m_textCaseIntact.c_str(), compare.m_textCaseIntact.c_str()) < 0;
            #else
            return strcmp(m_textCaseIntact.c_str(), compare.m_textCaseIntact.c_str()) < 0;
            #endif
		}
	}

	bool operator!=(std::string  compareString)
	{
		HashedCaseInsensitiveString compare(compareString);
		if (m_lowerCaseHash != compare.m_lowerCaseHash)
		{
			return true;
		}
		else
		{
            #ifndef __APPLE__
            return !(_stricmp(m_textCaseIntact.c_str(), compare.m_textCaseIntact.c_str()) == 0);
            #else
            return strcmp(m_textCaseIntact.c_str(), compare.m_textCaseIntact.c_str()) == 0;
            #endif
			
		}
	}

	unsigned int CalculateHash(std::string text)
	{
		unsigned int  hash= 0 ; 
		for (char letter : text)
		{
			hash *= 31;
			hash += (unsigned int)tolower(letter);
		}
		return hash;
	}
};

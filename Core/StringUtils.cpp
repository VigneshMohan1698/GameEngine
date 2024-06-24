#include "Engine/Core/StringUtils.hpp"
#include <stdarg.h>


//-----------------------------------------------------------------------------------------------
constexpr int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;


//-----------------------------------------------------------------------------------------------
const std::string Stringf( char const* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
const std::string Stringf( int maxLength, char const* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}

Strings SplitStringOnDelimiter(const std::string& originalString, char delimiterToSplitOn)
{
	Strings returnValue;
	bool delimiterFound = false;
	int count = 0;
	std::vector<int> positions;
	positions.push_back(0);
	count = 1;
	for (int i = 0; i < originalString.length(); i++)
	{
		if (originalString[i] == delimiterToSplitOn)
		{
			delimiterFound = true;
			positions.push_back(i);
			count++;
		}
	}
	if (delimiterFound)
	{
		for (int i = 0; i < count; i++)
		{
			if (i == (count - 1))
			{
				returnValue.push_back(originalString.substr(positions[i] + 1, originalString.length() - positions[i]));
			}
			else if (i == 0)
			{
				returnValue.push_back(originalString.substr(positions[i], positions[i + 1] -positions[i]));
			}
			else
			{
				returnValue.push_back(originalString.substr(positions[i] + 1, positions[i + 1] - positions[i] - 1));
			}


		}
	}
	else
	{
		returnValue.push_back(originalString);
	}
	return returnValue;

	
}







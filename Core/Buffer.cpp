
#include "Buffer.hpp"

BufferWriter::BufferWriter(Buffer& buffer)
{
	m_bufferObject = buffer;
}

void BufferWriter::AppendChar(const char& value)
{
	m_bufferObject.push_back(value);
}

void BufferWriter::AppendByte(unsigned char const& value)
{
	m_bufferObject.push_back(value);
}

void BufferWriter::AppendBool(bool const& value)
{
	bool valueBool = (unsigned char)value;
	m_bufferObject.push_back(valueBool);
}

void BufferWriter::AppendFloat(float const& value)
{
	unsigned char* floatArray = (unsigned char*) &value;
	if (GetPlatformNativeEndian() != m_endian)
	{
		Reverse4BytePrimitive(floatArray);
	}
	m_bufferObject.push_back(*floatArray);
	m_bufferObject.push_back(*(floatArray + 1));
	m_bufferObject.push_back(*(floatArray + 2));
	m_bufferObject.push_back(*(floatArray + 3));
}

void BufferWriter::AppendInt32(int32_t const& value)
{
	unsigned char* arrayOfBytes = (unsigned char*)&value;
	if (GetPlatformNativeEndian() != m_endian)
	{
		Reverse4BytePrimitive(arrayOfBytes);
	}
	m_bufferObject.push_back(*arrayOfBytes);
	m_bufferObject.push_back(*(arrayOfBytes + 1));
	m_bufferObject.push_back(*(arrayOfBytes + 2));
	m_bufferObject.push_back(*(arrayOfBytes + 3));
}

void BufferWriter::AppendDouble(const double& value)
{
	unsigned char* arrayOfBytes = (unsigned char*)&value;
	if (GetPlatformNativeEndian() != m_endian)
	{
		Reverse8BytePrimitive(arrayOfBytes);
	}
	m_bufferObject.push_back(*arrayOfBytes);
	m_bufferObject.push_back(*(arrayOfBytes + 1));
	m_bufferObject.push_back(*(arrayOfBytes + 2));
	m_bufferObject.push_back(*(arrayOfBytes + 3));
	m_bufferObject.push_back(*(arrayOfBytes + 4));
	m_bufferObject.push_back(*(arrayOfBytes + 5));
	m_bufferObject.push_back(*(arrayOfBytes + 6));
	m_bufferObject.push_back(*(arrayOfBytes + 7));
}

void BufferWriter::AppendStringZeroTerminated(std::string value)
{
	int length = (int)value.length() + 1;
	std::vector<unsigned char> data;
	data.resize(length);
	memcpy(&data[0], value.c_str(), length);
	for (int i = 0; i < length; i++)
	{
		m_bufferObject.push_back(data[i]);
	}
}

void BufferWriter::AppendStringAfter32BitLength(std::string value)
{
	AppendUint32((uint32_t)value.length());
	int length = (int)value.length();
	std::vector<unsigned char> data;
	data.resize(length);
	memcpy(&data[0], value.c_str(), length);
	for (int i = 0; i < length; i++)
	{
		m_bufferObject.push_back(data[i]);
	}
}

void BufferWriter::AppendRgba(Rgba8 const& value)
{
	m_bufferObject.push_back(value.r);
	m_bufferObject.push_back(value.g);
	m_bufferObject.push_back(value.b);
	m_bufferObject.push_back(value.a);
}

void BufferWriter::AppendRgb(Rgba8 const& value)
{
	m_bufferObject.push_back(value.r);
	m_bufferObject.push_back(value.g);
	m_bufferObject.push_back(value.b);
}

void BufferWriter::AppendIntVec2(IntVec2 const& value)
{
	AppendInt32(value.x);
	AppendInt32(value.y);
}

void BufferWriter::AppendVec2(Vec2 const& value)
{
	AppendFloat(value.x);
	AppendFloat(value.y);
}

void BufferWriter::AppendVec3(Vec3 const& value)
{
	AppendFloat(value.x);
	AppendFloat(value.y);
	AppendFloat(value.z);
	/*unsigned char* arrayOfBytesx = (unsigned char*)&value.x;
	m_bufferObject.push_back(*arrayOfBytesx);
	m_bufferObject.push_back(*(arrayOfBytesx + 1));
	m_bufferObject.push_back(*(arrayOfBytesx + 2));
	m_bufferObject.push_back(*(arrayOfBytesx + 3));
	unsigned char* arrayOfBytesy = (unsigned char*)&value.y;
	m_bufferObject.push_back(*arrayOfBytesy);
	m_bufferObject.push_back(*(arrayOfBytesy + 1));
	m_bufferObject.push_back(*(arrayOfBytesy + 2));
	m_bufferObject.push_back(*(arrayOfBytesy + 3));
	unsigned char* arrayOfBytesz = (unsigned char*)&value.z;
	m_bufferObject.push_back(*arrayOfBytesz);
	m_bufferObject.push_back(*(arrayOfBytesz + 1));
	m_bufferObject.push_back(*(arrayOfBytesz + 2));
	m_bufferObject.push_back(*(arrayOfBytesz + 3));*/
}

void BufferWriter::AppendVertexPCU(Vertex_PCU const& value)
{
	AppendVec3(value.m_position);
	AppendRgba(value.m_color);
	AppendVec2(value.m_uvTexCoords);
}

EndianNess GetPlatformNativeEndian()
{
	unsigned int x = 0x12345678;
	unsigned char* firstByte = (unsigned char*) &x;
	if (*firstByte == 12)
	{
		return EndianNess::BigEndian; 
	}
	else 
		return EndianNess::LittleEndian;
}

void Reverse4BytePrimitive(unsigned char* value) {
	std::vector<unsigned char> temp;
	temp.resize(4);
	memcpy(&temp[0], value, 4);
	value[0] = temp[3];
	value[1] = temp[2];
	value[2] = temp[1];
	value[3] = temp[0];
}

void  Reverse8BytePrimitive(unsigned char* value) {
	std::vector<unsigned char> temp;
	temp.resize(8);
	memcpy(&temp[0], value, 8);
	value[0] = temp[7];
	value[1] = temp[6];
	value[2] = temp[5];
	value[3] = temp[4];
	value[4] = temp[3];
	value[5] = temp[2];
	value[6] = temp[1];
	value[7] = temp[0];
}

void BufferWriter::AppendUint32(uint32_t const& value)
{
	unsigned char* uint32Array = (unsigned char*)&value;
	if (GetPlatformNativeEndian() != m_endian)
	{
		Reverse4BytePrimitive(uint32Array);
	}
	m_bufferObject.push_back(*uint32Array);
	m_bufferObject.push_back(*(uint32Array + 1));
	m_bufferObject.push_back(*(uint32Array + 2));
	m_bufferObject.push_back(*(uint32Array + 3));
}

void BufferWriter::UpdateUint32AtPosition(int index, uint32_t value)
{
	unsigned char* dataPoint = &m_bufferObject[0];
	dataPoint += index;
	memcpy (dataPoint,&value, 4);
}


BufferParser::BufferParser(const unsigned char* bufferData, int size, EndianNess endian)
{
	m_bufferData = bufferData;
	m_endian = endian;
	size = size;
}

BufferParser::BufferParser(const unsigned char* bufferData, EndianNess endian)
{
	m_bufferData = bufferData;
	m_endian = endian;
}

char BufferParser::ParseChar()
{
	char* value = (char*)m_bufferData;
	m_bufferData += 1;
	return *value;
}
unsigned char BufferParser::ParseByte() 
{
	unsigned char* value = (unsigned char*)m_bufferData;
	m_bufferData += 1;
	return  *value;
}
unsigned char BufferParser::ParseBool()
{
	unsigned char* value = (unsigned char*)m_bufferData;
	m_bufferData += 1;
	return  *value;
}
float BufferParser::ParseFloat()
{
	const float* value = reinterpret_cast<const float*>(m_bufferData);
	m_bufferData += 4;
	return  *value;
}
uint32_t BufferParser::ParseUint32()
{
	const uint32_t* value = reinterpret_cast<const uint32_t*>(m_bufferData);
	m_bufferData += 4;
	return  *value;
}
int32_t BufferParser::ParseInt32()
{
	const int32_t* value = reinterpret_cast<const int32_t*>(m_bufferData);
	m_bufferData += 4;
	return  *value;
}

double BufferParser::ParseDouble()
{
	const double* value = reinterpret_cast<const double*>(m_bufferData);
	m_bufferData += 8;
	return  *value;
}
void BufferParser::ParseStringZeroTerminated(std::string data)
{
	char dataChar; 
	do
	{
		dataChar = ParseChar();
		data.push_back(dataChar);
	}
	while(dataChar != '\0');
} 

void BufferParser::ParseStringAfter32BitLength(std::string data)
{
	int datasize = ParseUint32();
	
	for (int i = 0; i < datasize; i++)
	{
		char dataChar = ParseChar();
		data.push_back(dataChar);
	}
} 
Rgba8 BufferParser::ParseRgba()
{
	Rgba8 value;
	const unsigned char* r = reinterpret_cast<const unsigned char*>(m_bufferData);
	value.r = *r;
	m_bufferData+=1;
	const unsigned char* g = reinterpret_cast<const unsigned char*>(m_bufferData);
	value.g = *g;
	m_bufferData += 1;
	const unsigned char* b = reinterpret_cast<const unsigned char*>(m_bufferData);
	value.b = *b;
	m_bufferData += 1;
	const unsigned char* a = reinterpret_cast<const unsigned char*>(m_bufferData);
	value.a = *a;
	m_bufferData += 1;
	return value;
} 
Rgba8 BufferParser::ParseRgb()
{
	Rgba8 value;
	const unsigned char* r = reinterpret_cast<const unsigned char*>(m_bufferData);
	value.r = *r;
	m_bufferData += 1;
	const unsigned char* g = reinterpret_cast<const unsigned char*>(m_bufferData);
	value.g = *g;
	m_bufferData += 1;
	const unsigned char* b = reinterpret_cast<const unsigned char*>(m_bufferData);
	value.b = *b;
	m_bufferData += 1;
	return value;
}
IntVec2 BufferParser::ParseIntVec2()
{
	IntVec2 value;
	value.x = ParseInt32();
	value.y = ParseInt32();
	return value;
}
Vec2 BufferParser::ParseVec2()
{
	Vec2 value;
	value.x = ParseFloat();
	value.y = ParseFloat();
	return value;
}
Vec3 BufferParser::ParseVec3()
{
	Vec3 value;
	value.x = ParseFloat();
	value.y = ParseFloat();
	value.z = ParseFloat();
	return value;
}
Vertex_PCU BufferParser::ParseVertexPCU()
{
	Vertex_PCU vertexPCU;
	vertexPCU.m_position = ParseVec3();
	vertexPCU.m_color = ParseRgba();
	vertexPCU.m_uvTexCoords = ParseVec2();
	return vertexPCU;
}


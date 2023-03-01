#pragma once
#include <vector>
#include <string>
#include <Engine/Core/Rgba8.hpp>
#include <Engine/Math/IntVec2.hpp>
#include <Engine/Core/Vertex_PCU.hpp>

enum class EndianNess
{
	BigEndian,
	LittleEndian
};
typedef std::vector<unsigned char> Buffer;

EndianNess GetPlatformNativeEndian();
void  Reverse4BytePrimitive(unsigned char* value);
void  Reverse8BytePrimitive(unsigned char* value);

class BufferWriter
{
	public:
		BufferWriter();
		BufferWriter(Buffer& buffer);
		void AppendChar(const char& value);
		void AppendByte(unsigned char const& value);
		void AppendBool(bool const& value);
		void AppendFloat(float const& value);
		void AppendUint32(uint32_t const& value);
		void AppendInt32(int32_t const& value);
		void SetEndianMode(EndianNess endian) { m_endian  = endian;};
		void AppendDouble(const double& value); // actually 3.1415926535897931 (best it can do)
		void AppendStringZeroTerminated(std::string value); // written with a trailing 0 ('\0') after (6 bytes total)
		void AppendStringAfter32BitLength(std::string value); // uint 17, then 17 chars (no zero-terminator after)
		void AppendRgba(Rgba8 const& value); // four bytes in RGBA order (endian-independent)
		void AppendRgb(Rgba8 const& value); // four bytes in RGBA order (endian-independent)
		void AppendIntVec2(IntVec2 const& value);
		void AppendVec2(Vec2 const& value);
		void AppendVec3(Vec3 const& value);
		void AppendVertexPCU(Vertex_PCU const& value);
		void UpdateUint32AtPosition(int index, uint32_t value);
		int GetEndianMode() {return (int)m_endian;}; 
	public:
		EndianNess m_endian = EndianNess::LittleEndian;
		Buffer m_bufferObject;
};
class BufferParser
{
	public:
		BufferParser(const unsigned char* bufferData, int size, EndianNess endian = EndianNess::LittleEndian);
		BufferParser(const unsigned char* bufferData,EndianNess endian = EndianNess::LittleEndian);

		char ParseChar();
		unsigned char ParseByte();
		unsigned char ParseBool();
		float ParseFloat();
		uint32_t ParseUint32();
		int32_t ParseInt32();
		void SetEndianMode(EndianNess endian) { m_endian = endian; };
		double ParseDouble(); // actually 3.1415926535897931 (best it can do)
		void ParseStringZeroTerminated(std::string data); // written with a trailing 0 ('\0') after (6 bytes total)
		void ParseStringAfter32BitLength(std::string data); // uint 17, then 17 chars (no zero-terminator after)
		Rgba8 ParseRgba(); // four bytes in RGBA order (endian-independent)
		Rgba8 ParseRgb(); // four bytes in RGBA order (endian-independent)
		IntVec2 ParseIntVec2();
		Vec2 ParseVec2();
		Vec3 ParseVec3();
		Vertex_PCU ParseVertexPCU();
		void RollParser(int bytesToRoll) { m_bufferData  += bytesToRoll;}
	public:
		EndianNess m_endian = EndianNess::LittleEndian;
		const unsigned char* m_bufferData;
		int size = 0;
};
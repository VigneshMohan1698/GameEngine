#ifndef __APPLE__
#include "FileUtils.hpp"
#include <Engine/Core/ErrorWarningAssert.hpp>
#include <Engine/Core/EngineCommon.hpp>
#include "Engine/Core/Vertex_PNCU.hpp"

bool FileExists(const std::string& filename)
{
	struct stat buf;
	return (stat(filename.c_str(), &buf) == 0);
}

int FileWriteFromBuffer(std::vector<uint8_t>& inBuffer, const std::string& filename)
{
	size_t itemsWritten = 0;
	FILE* filestream;
	fopen_s(&filestream, filename.c_str(), "wb");
	size_t dataSize = sizeof(uint8_t);
	size_t count = (size_t)inBuffer.size();
	itemsWritten = fwrite(inBuffer.data(), dataSize, count,filestream);
	fclose(filestream);
	//else
	//{
	//	ERROR_AND_DIE(filename + " : Does Not exist, Error in FileWriteFromBuffer");
	//	return 0;
	//}
	return 0;
}
int FileWriteFromBuffer(std::vector<float>& inBuffer, const std::string& filename)
{
	size_t itemsWritten = 0;
	FILE* filestream;
	fopen_s(&filestream, filename.c_str(), "wb");
	size_t dataSize = sizeof(float);
	size_t count = (size_t)inBuffer.size();
	itemsWritten = fwrite(inBuffer.data(), dataSize, count, filestream);
	fclose(filestream);
	return 0;
}

int FileWriteFromBuffer(std::vector<Vertex_PNCU>& inBuffer, const std::string& filename)
{
	size_t itemsWritten = 0;
	FILE* filestream;
	fopen_s(&filestream, filename.c_str(), "wb");
	size_t dataSize = sizeof(Vertex_PNCU);
	size_t count = (size_t)inBuffer.size();
	itemsWritten = fwrite(inBuffer.data(), dataSize, count, filestream);
	fclose(filestream);
	return 0;
}

int FileReadToBuffer(std::vector<uint8_t>& outBuffer, const std::string& filename)
{
	int itemsRead = 0;
	if (FileExists(filename))
	{
		long lSize;
		FILE* filestream;
		fopen_s(&filestream, filename.c_str(), "rb");
		fseek(filestream, 0L, SEEK_END);
		lSize = FileSize(filestream);
		outBuffer.resize(lSize + 1);
		fseek(filestream, 0L, SEEK_SET);
		fread(outBuffer.data(), lSize, 1, filestream);
		rewind(filestream);
		fclose(filestream);
	}
	else
	{
		ERROR_AND_DIE(filename + " : Does Not exist, Error in FileReadToBuffer");
	}
	return itemsRead;
}
int FileReadToBuffer(std::vector<float>& outBuffer, const std::string& filename)
{
	int itemsRead = 0;
	if (FileExists(filename))
	{
		long lSize;
		FILE* filestream;
		fopen_s(&filestream, filename.c_str(), "rb");
		fseek(filestream, 0L, SEEK_END);
		lSize = FileSize(filestream);
		outBuffer.resize(lSize + 1);
		fseek(filestream, 0L, SEEK_SET);
		fread(outBuffer.data(), sizeof(float),lSize, filestream);
		rewind(filestream);
		fclose(filestream);
	}
	else
	{
		ERROR_AND_DIE(filename + " : Does Not exist, Error in FileReadToBuffer");
	}
	return itemsRead;
}

int FileReadToBuffer(std::vector<Vertex_PNCU>& outBuffer, const std::string& filename)
{
	int itemsRead = 0;
	if (FileExists(filename))
	{
		long lSize;
		FILE* filestream;
		fopen_s(&filestream, filename.c_str(), "rb");
		fseek(filestream, 0L, SEEK_END);
		lSize = FileSize(filestream) / sizeof(Vertex_PNCU);
		outBuffer.resize(lSize);
		fseek(filestream, 0L, SEEK_SET);
		fread(outBuffer.data(), sizeof(Vertex_PNCU), lSize, filestream);
		rewind(filestream);
		fclose(filestream);
	}
	else
	{
		ERROR_AND_DIE(filename + " : Does Not exist, Error in FileReadToBuffer");
	}
	return itemsRead;
}
int FileReadToString(std::string& outString, const std::string& filename)
{
	std::vector<uint8_t> outBuffers;
	int itemsRead = 0;
	itemsRead = FileReadToBuffer(outBuffers, filename);
	//std::string outStrings(outBuffers.begin(), outBuffers.end());
	outBuffers.push_back('\0');
	outString.assign(outBuffers.begin(), outBuffers.end());
	return itemsRead;
}

int FileSize(FILE* fileStream) 
{
	int prev = ftell(fileStream);
	fseek(fileStream, 0L, SEEK_END);
	int sz = ftell(fileStream);
	fseek(fileStream, prev, SEEK_SET); 
	return sz;
}
#endif

#ifndef __APPLE__
#pragma once
#include <string>
#include <vector>
struct Vertex_PNCU;

bool FileExists(const std::string& filename);
int FileWriteFromBuffer(std::vector<uint8_t>& inBuffer, const std::string& filename);
int FileWriteFromBuffer(std::vector<float>& inBuffer, const std::string& filename);
int FileWriteFromBuffer(std::vector<Vertex_PNCU>& inBuffer, const std::string& filename);
int FileReadToBuffer(std::vector<uint8_t>& outBuffer, const std::string& filename);
int FileReadToBuffer(std::vector<Vertex_PNCU>& outBuffer, const std::string& filename);
int FileReadToBuffer(std::vector<float>& outBuffer, const std::string& filename);
int FileReadToString(std::string& outString, const std::string& filename);
int FileSize(FILE* fileStream);
#endif

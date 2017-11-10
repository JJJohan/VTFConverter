#pragma once

#include <VTFLib13/VTFWrapper.h>
#include <vector>
#include <string>

class VTFConverter
{
public:
	VTFConverter(int width, int height);
	~VTFConverter();

	std::vector<char> ReadData(std::vector<char> inputData);
	static void LogError(const std::string& error);
	void LogResults() const;

private:
	int _width;
	int _height;
	int _fileSize;
	std::string _originalFormat;
	vlUInt _image;
	vlUInt _material;
};

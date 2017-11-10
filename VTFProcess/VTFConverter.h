#pragma once

#include <VTFLib13/VTFWrapper.h>
#include <vector>
#include <string>

class VTFConverter
{
public:
	VTFConverter(int width, int height);
	~VTFConverter();

	std::vector<char> ReadData(std::vector<char> inputData) const;
	static void LogError(const std::string& error);

private:
	int _width;
	int _height;
	vlUInt _image;
	vlUInt _material;
};

#pragma once

#include <VTFLib13/VTFWrapper.h>
#include <vector>

class VTFConverter
{
public:
	VTFConverter(int width, int height);
	~VTFConverter();

	bool ReadData(std::vector<char> inputData);

private:
	int _width;
	int _height;
	vlUInt _image;
	vlUInt _material;
};

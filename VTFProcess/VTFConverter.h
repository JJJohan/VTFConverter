#pragma once

#include "VTFLib13/VTFWrapper.h"
#include <vector>

class VTFConverter
{
public:
	VTFConverter();
	~VTFConverter();

	bool ReadData(std::vector<char> inputData);

private:
	vlUInt uiVTFImage;
	vlUInt uiVMTMaterial;
};

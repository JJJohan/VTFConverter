#include "VTFConverter.h"
#include "VTFLib13/VTFLib.h"
#include "VTFLib13/VMTWrapper.h"
#include <iostream>

VTFConverter::VTFConverter()
{
	std::cout << "Initialising VTF Converter." << std::endl;
	vlInitialize();

	vlCreateImage(&uiVTFImage);
	vlBindImage(uiVTFImage);

	vlCreateMaterial(&uiVMTMaterial);
	vlBindMaterial(uiVMTMaterial);
}

bool VTFConverter::ReadData(std::vector<char> inputData)
{
	return false;
}

VTFConverter::~VTFConverter()
{
	std::cout << "Shutting down VTF Converter." << std::endl;

	vlDeleteMaterial(uiVMTMaterial);
	vlDeleteImage(uiVTFImage);
	vlShutdown();
}

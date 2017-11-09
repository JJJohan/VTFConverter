#include "VTFConverter.h"
#include <VTFLib13/VTFLib.h>
#include <VTFLib13/VMTWrapper.h>
#include <ImageMagick-6/Magick++/Image.h>
#include <ImageMagick-6/Magick++/Exception.h>
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
	Magick::Image image;
	
	try
	{
		const Magick::Blob blob(inputData.data(), inputData.size());
		image.read(blob);
		std::cout << "Data read! Format: " << image.format() << std::endl;
	}
	catch (Magick::Exception& error)
	{
		std::cout << error.what() << std::endl;
		return false;
	}

	return true;
}

VTFConverter::~VTFConverter()
{
	std::cout << "Shutting down VTF Converter." << std::endl;

	vlDeleteMaterial(uiVMTMaterial);
	vlDeleteImage(uiVTFImage);
	vlShutdown();
}

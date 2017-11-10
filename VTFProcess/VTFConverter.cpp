#include "VTFConverter.h"
#include <VTFLib13/VTFLib.h>
#include <VTFLib13/VMTWrapper.h>
#include <ImageMagick-6/Magick++/Image.h>
#include <ImageMagick-6/Magick++/Exception.h>
#include <iostream>

VTFConverter::VTFConverter(const int width, const int height)
	: _width(width)
	, _height(height)
{
	std::cout << "Initialising VTF Converter." << std::endl;
	vlInitialize();

	vlCreateImage(&_image);
	vlBindImage(_image);

	vlCreateMaterial(&_material);
	vlBindMaterial(_material);
}

bool VTFConverter::ReadData(std::vector<char> inputData)
{
	Magick::Image image;
	
	try
	{
		// Read and resize the input image.
		const Magick::Blob readBuffer(inputData.data(), inputData.size());
		image.read(readBuffer);
		image.resize(Magick::Geometry(_width, _height));

		// Read into RGBA buffer.
		Magick::Blob rgbaBuffer;
		image.write(rgbaBuffer);

		SVTFCreateOptions createOptions;
		createOptions.ImageFormat = image.format() == "rgba" ? IMAGE_FORMAT_DXT5 : IMAGE_FORMAT_DXT1;

		// Convert to VTF.
		if (!vlImageCreateSingle(_width, _height, rgbaBuffer.data(), &createOptions))
		{
			std::cout << "Failed to convert to VTF." << std::endl;
			return false;
		}
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

	vlDeleteMaterial(_material);
	vlDeleteImage(_image);
	vlShutdown();
}

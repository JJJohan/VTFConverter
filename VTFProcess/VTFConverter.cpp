#include "VTFConverter.h"
#include <VTFLib13/VTFLib.h>
#include <VTFLib13/VMTWrapper.h>
#include <FreeImage.h>
#include <iostream>
#include <fstream>
#include <vector>

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

	FreeImage_Initialise();
}

std::vector<char> VTFConverter::ReadData(std::vector<char> inputData)
{	
	// Read the image.
	FIMEMORY* imageMemory = FreeImage_OpenMemory(reinterpret_cast<BYTE*>(inputData.data()), DWORD(inputData.size()));
	const FREE_IMAGE_FORMAT imageType = FreeImage_GetFileTypeFromMemory(imageMemory, 0);
	FIBITMAP* image = FreeImage_LoadFromMemory(imageType, imageMemory);
	FreeImage_CloseMemory(imageMemory);

	const bool isAlpha = FreeImage_IsTransparent(image);

	// Resize the image.
	image = FreeImage_Rescale(image, _width, _height);
	FreeImage_FlipVertical(image);

	// Write image data to raw buffer.
	image = FreeImage_ConvertTo32Bits(image);
	BYTE* pixelData = FreeImage_GetBits(image);
	FreeImage_Unload(image);

	// Convert BGRA > RGBA.
	size_t totalSize = _width * _height * 4;
	for (size_t i = 0; i < totalSize; i += 4)
	{
		size_t i2 = i + 2;
		const BYTE tmp = pixelData[i2];
		pixelData[i2] = pixelData[i];
		pixelData[i] = tmp;
	}

	// Prepare VTF options.
	SVTFCreateOptions createOptions;
	createOptions.uiVersion[0] = 7;
	createOptions.uiVersion[1] = 5;
	createOptions.bNormalMap = false;
	createOptions.bMipmaps = false;
	createOptions.ImageFormat = isAlpha ? IMAGE_FORMAT_DXT5 : IMAGE_FORMAT_DXT1;

	// Convert to VTF.
	const vlBool createImage = vlImageCreateSingle(_width, _height, static_cast<vlByte*>(pixelData), &createOptions);
	
	if (!createImage)
	{
		std::cout << vlGetLastError() << std::endl;
		return std::vector<char>();
	}

	vlUInt vtfSize = vlImageGetSize();
	std::vector<char> output(vtfSize);

	vlSize unused;
	if (!vlImageSaveLump(output.data(), vtfSize, &unused))
	{
		std::cout << vlGetLastError() << std::endl;
		return std::vector<char>();
	}
	
	return output;
}

VTFConverter::~VTFConverter()
{
	std::cout << "Shutting down VTF Converter." << std::endl;

	FreeImage_DeInitialise();

	vlDeleteMaterial(_material);
	vlDeleteImage(_image);
	vlShutdown();
}

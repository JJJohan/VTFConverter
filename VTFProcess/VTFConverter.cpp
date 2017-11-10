#include "VTFConverter.h"
#include <VTFLib13/VTFLib.h>
#include <VTFLib13/VMTWrapper.h>
#include <FreeImage.h>
#include <fstream>
#include <vector>
#include <iostream>

VTFConverter::VTFConverter(const int width, const int height)
	: _width(width)
	, _height(height)
    , _fileSize(0)
{
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

	_originalFormat = FreeImage_GetFormatFromFIF(imageType);

	const bool isAlpha = FreeImage_IsTransparent(image);
	image = FreeImage_ConvertTo32Bits(image);

	// Resize the image.
	image = FreeImage_Rescale(image, _width, _height);
	FreeImage_FlipVertical(image);

	// Write image data to raw buffer.
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
		LogError(vlGetLastError());
		return std::vector<char>();
	}

	_fileSize = vlImageGetSize();
	std::vector<char> output(_fileSize);

	vlSize unused;
	if (!vlImageSaveLump(output.data(), _fileSize, &unused))
	{
		LogError(vlGetLastError());
		return std::vector<char>();
	}

	return output;
}

void VTFConverter::LogResults() const
{
	// Some manually printed JSON, no need for a third party library here.
	std::ofstream logStream("output.json");
	logStream << "{" << std::endl;
	logStream << "\t\"success\": true," << std::endl;
	logStream << "\t\"fileSize\": " << _fileSize << "," << std::endl;
	logStream << "\t\"originalFormat\": \"" << _originalFormat << "\"" << std::endl;
	logStream << "}" << std::endl;
	logStream.close();
}

void VTFConverter::LogError(const std::string& string)
{
	// Some manually printed JSON, no need for a third party library here.
	std::cout << string << std::endl;
	std::ofstream logStream("output.json");
	logStream << "{" << std::endl;
	logStream << "\t\"success\": false," << std::endl;
	logStream << "\t\"message\": \"" << string << "\"" << std::endl;
	logStream << "}" << std::endl;
	logStream.close();
}

VTFConverter::~VTFConverter()
{
	FreeImage_DeInitialise();

	vlDeleteMaterial(_material);
	vlDeleteImage(_image);
	vlShutdown();
}

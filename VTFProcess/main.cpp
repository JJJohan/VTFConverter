#include "VTFConverter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>

int StringToInt(const std::string string)
{
	std::stringstream stream(string);
	int integer;
	stream >> integer;
	return integer;
}

bool IsPowerOfTwo(const int value)
{
	return (value & (value - 1)) == 0;
}

int main(int argc, char* argv[])
{
	remove("output.log");
	remove("output.vtf");

	if (argc < 2)
	{
		VTFConverter::LogError("No file path provided.");
		return 1;
	}

	const std::string filePath = argv[1];
	std::string width = "256";
	std::string height = "256";
	if (argc == 4)
	{
		width = argv[2];
		height = argv[3];
	}

	const int widthVal = StringToInt(width);
	const int heightVal = StringToInt(height);

	if (widthVal == 0 || heightVal == 0)
	{
		VTFConverter::LogError("Invalid values given for requested image dimensions.");
		return 1;
	}

	if (!IsPowerOfTwo(widthVal) || !IsPowerOfTwo(heightVal))
	{
		VTFConverter::LogError("Width and height both need to be power of two.");
		return 1;
	}

	// Open file.
	std::ifstream inputFile(filePath, std::ios::in | std::ios::binary | std::ios::ate);
	if (inputFile.is_open())
	{
		const std::streamsize size = inputFile.tellg();
		inputFile.seekg(0, std::ios::beg);

		std::vector<char> buffer(size);
		if (inputFile.read(buffer.data(), size))
		{
			// Convert the file.
			VTFConverter converter(widthVal, heightVal);
			std::vector<char> output = converter.ReadData(buffer);
			if (!output.empty())
			{
				std::ofstream outStream("output.vtf");
				outStream.write(output.data(), output.size());
				outStream.close();
			}
			else
			{
				VTFConverter::LogError("Data provided is not valid or not a supported image format.");
				return 1;
			}
		}
		else
		{
			VTFConverter::LogError(std::string("Failed to read file: ") + std::string(argv[1]));
			return 1;
		}
	}
	else
	{
		VTFConverter::LogError(std::string("Failed to open file: ") + std::string(argv[1]));
		return 1;
	}

	return 0;
}

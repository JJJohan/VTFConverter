#include "VTFConverter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int StringToInt(std::string string)
{
	std::stringstream stream(string);
	int integer;
	stream >> integer;
	return integer;
}

int main(int argc, char* argv[])
{
	std::cout << "VTF Process" << std::endl;

	if (argc < 2)
	{
		std::cout << "No file path provided." << std::endl;
		return 1;
	}

	//std::string filePath = argv[1];
	std::string width = "256";
	std::string height = "256";
	if (argc == 4)
	{
		width = argv[3];
		height = argv[4];
	}

	std::string filePath = "/mnt/c/Users/admin/Pictures/after.png";
	int widthVal = StringToInt(width);
	int heightVal = StringToInt(height);

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
			if (converter.ReadData(buffer))
			{
				
			}
			else
			{
				std::cout << "Data provided is not a valid or supported image format." << std::endl;
				return 1;
			}
		}
		else
		{
			std::cout << "Failed to read file: " << argv[1] << std::endl;
			return 1;
		}
	}
	else
	{
		std::cout << "Failed to open file: " << argv[1] << std::endl;
		return 1;
	}
	
    return 0;
}

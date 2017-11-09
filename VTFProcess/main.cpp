#include <iostream>
#include <fstream>
#include "VTFConverter.h"
#include <vector>

int main(int argc, char* argv[])
{
	std::cout << "VTF Process" << std::endl;

	/*if (argc != 2)
	{
		std::cout << "No file path provided." << std::endl;
		return 1;
	}*/

	//std::string filePath = argv[1];
	std::string filePath = "/mnt/c/Users/admin/Pictures/after.jpg";

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
			VTFConverter converter;
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

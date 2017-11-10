#include <iostream>
#include <string>
#include "IL/devil_cpp_wrapper.hpp"

int main()
{
	std::string testImage = "C:\\Users\\admin\\Documents\\Repositories\\VTFConverter\\VTFConverter\\test.png";
	std::string testOutput = "C:\\Users\\admin\\Documents\\Repositories\\VTFConverter\\VTFConverter\\test.vtf";

	std::cout << "VTF converter" << std::endl;
	ilImage ilImage;
	ilState ilState;
	ilError ilError;

	if (ilImage.Load(testImage.c_str()))
	{
		//ilImage.Resize(256, 256, ilImage.Depth());
		ilImage.Bind();

		ilSetInteger(IL_VTF_COMP, IL_DXT5);
		ilState.Enable(IL_FILE_OVERWRITE);
		
		if (!ilImage.Save(testOutput.c_str()))
		{

		}

		std::cout << ilError.String() << std::endl;
		ilImage.Close();
	}

	std::cin.get();
    return 0;
}
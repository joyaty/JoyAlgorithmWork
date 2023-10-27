
#include "TextQuery/TextQuery.h"
#include <fstream>
#include <iostream>
#if defined(__APPLE__)
    #include <unistd.h>
    #define GetCurrentDir getcwd
#elif defined(WIN32)
    #include <direct.h>
    #define GetCurrentDir _getcwd
#endif

std::string GetCurrentWorkPath()
{
    char buffer[256];
    char* ret = GetCurrentDir(buffer, 256);
    std::string strCurrentPath(buffer);
    return strCurrentPath;
}

int main()
{
	std::ifstream fs("TestDocument.txt");
    TextGraph textGraph(fs);
    
    std::cout << GetCurrentWorkPath() << std::endl;
    
    std::cin.get();
	return 0;
}

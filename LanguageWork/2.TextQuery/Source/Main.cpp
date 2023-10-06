
#include "TextQuery/TextQuery.h"
#include <fstream>
#include <iostream>
#if defined(__APPLE__)
    #include <unistd.h>
    #define GetCurrentDir getcwd
#elif defined(_MSV_VER_)
    #include <dirct.h>
    #define GetCurrentDir _getcwd
#endif

std::string GetCurrentPath()
{
    char buffer[256];
    getcwd(buffer, 256);
    std::string strCurrentPath(buffer);
    return strCurrentPath;
}

int main()
{
	std::ifstream fs("TestDocument.txt");
    TextGraph textGraph(fs);
    
    std::cout << GetCurrentPath() << std::endl;
    
    std::cin.get();
	return 0;
}

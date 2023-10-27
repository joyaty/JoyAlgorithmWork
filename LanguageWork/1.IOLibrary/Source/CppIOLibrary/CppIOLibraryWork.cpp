
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

#if defined(__APPLE__)
#include <unistd.h>
#define GetCurrentDir getcwd
#elif defined(WIN32)
#include <direct.h>
#define GetCurrentDir _getcwd
#endif

void TestCIOLibraryWork();

std::string GetResourcePath()
{
	char pathBuffer[256];
	const char* strPath = GetCurrentDir(pathBuffer, 256);
	std::string strWorkPath(pathBuffer);
	int index = strWorkPath.find("Build");
	std::string strRootPath = strWorkPath.substr(0, index);
	return std::string(strRootPath.append("LanguageWork\\1.IOLibrary\\Resources\\"));
}

/// <summary>
/// C++ IO标准库
/// </summary>
void TestCppIOLibraryWork()
{
	std::string strWorkPath = GetResourcePath();
	std::cout << "Resource Path: " << strWorkPath << std::endl;
	std::ifstream inputFileStream(strWorkPath + "TestIOFile.txt");
	int fileSize = inputFileStream.seekg(-5, std::ios::end).tellg();
	std::cout << fileSize << std::endl;
	std::cout << inputFileStream.rdbuf() << std::endl;
	inputFileStream.close();

	TestCIOLibraryWork();
}

/// <summary>
/// C风格IO库
/// </summary>
void TestCIOLibraryWork()
{
	std::cout << "--------- C IO Library ---------" << std::endl;

	std::string strWorkPath = GetResourcePath();
	std::cout << "Resource Path: " << strWorkPath << std::endl;

	FILE* pFile = fopen((strWorkPath + "TestIOFile.txt").c_str(), "r");

	char contentBuffer[256];
	fread(contentBuffer, sizeof(char), 256, pFile);

	std::string strContent(contentBuffer);
	std::cout << strContent << std::endl;
}

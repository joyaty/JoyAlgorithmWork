
#include "TextQuery.h"
#include <iostream>
#include <sstream>

TextGraph::TextGraph(std::ifstream& iFileStream)
{
	std::string strContent;
    std::cout << iFileStream.good() << ", " << iFileStream.fail() << ", " << iFileStream.eof() << ", " << iFileStream.rdstate() << std::endl;
    // getline(iFileStream, strContent);
	while (getline(iFileStream, strContent))
	{
        m_vecContents.push_back(strContent);
        Line_no lineNo = m_vecContents.size() - 1;
        std::istringstream strStream(strContent);
        std::string word;
        while (strStream >> word)
        {
            m_mapWordLineNos[word].emplace(lineNo);
        }
	}
    bool isBad = (iFileStream.rdstate() & std::ifstream::badbit) == std::ifstream::badbit;
    bool isEof = (iFileStream.rdstate() & std::ifstream::eofbit) == std::ifstream::eofbit;
    bool isFail = (iFileStream.rdstate() & std::ifstream::failbit) == std::ifstream::failbit;
    bool isGood = (iFileStream.rdstate() & std::ifstream::goodbit) == std::ifstream::goodbit;
    std::cout << iFileStream.good() << ", " << iFileStream.fail() << ", " << iFileStream.eof() << ", " << iFileStream.bad() << ", " << iFileStream.rdstate() << std::endl;
    std::cout << isGood << ", " << isFail << ", " << isEof << ", " << isBad << std::endl;
    
}

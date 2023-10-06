
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <fstream>

class QueryResult;

/// <summary>
/// 读取文本，生成文本查询图
/// </summary>
class TextGraph
{
public:
	using Line_no = std::vector<std::string>::size_type;

public:
	TextGraph(std::ifstream& iFileStream);

public:
	// 查询关键词出现的行，以及对应的行号
	QueryResult Query(const std::string& strKeyword) const;

private:
	// 记录每行的文本内容
	std::vector<std::string> m_vecContents{};
	// 单词与出现的行号集合映射
	std::map<std::string, std::set<Line_no>> m_mapWordLineNos{};
};

class QueryResult
{
public:
	struct LineAndContent
	{
		TextGraph::Line_no nLineNo;
		std::string strContent;
	};

private:
	std::string m_strKeyWord;
	std::vector<LineAndContent> m_vecResultGroup;
};

class QueryBase
{
public:
	virtual ~QueryBase();

public:
	
};

class WordQuery : public QueryBase
{
};

class NotQure : public QueryBase
{

};

class BinaryQuery : public QueryBase
{

};

class AndQuery : public BinaryQuery
{

};

class OrQuery : public BinaryQuery
{

};

class Query
{
public:
	Query(const std::string& strKeyword);

private:
	std::shared_ptr<QueryBase> m_pQuery;
};

/// <summary>
/// 重载操作符~，查询结果取非
/// </summary>
/// <param name="query"></param>
/// <returns></returns>
Query operator~ (const Query& query);

/// <summary>
/// 重载操作符&，两个查询关系取且
/// </summary>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
Query operator& (const Query& lhs, const Query& rhs);

/// <summary>
/// 重载操作符，两个查询关系取或
/// </summary>
/// <param name="lhs"></param>
/// <param name="rhs"></param>
/// <returns></returns>
Query operator| (const Query& lhs, const Query& rhs);

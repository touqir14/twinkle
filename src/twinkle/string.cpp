#include "string.h"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace twinkle
{
	string::string()
		: std::string()
	{

	}

	string::string(const std::string& t_str)
		: std::string(t_str)
	{

	}

	std::vector<string> string::split(const char t_delimiter) const
	{
		std::vector<string> tokens;
		string token;
		std::istringstream tokenStream(*this);
		while (std::getline(tokenStream, token, t_delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}

	bool string::contains(const char t_match) const
	{
		return find(t_match) != std::string::npos;
	}

	bool string::contains(const std::string& t_match) const
	{
		return find(t_match) != std::string::npos;
	}

	bool string::startsWith(const std::string& t_match,	const CompareMode t_mode) const
	{
		if (t_mode == CompareMode::CaseSensitive)
		{
			return find(t_match) == 0;
		}
		else
		{
			return toLower().find(string(t_match).toLower()) == 0;
		}
	}

	bool string::endsWith(const std::string& t_match, const CompareMode t_mode) const
	{
		return false;
	}

	string string::toLower() const
	{
		string result(*this);
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}

	string string::toUpper() const
	{
		string result(*this);
		std::transform(result.begin(), result.end(), result.begin(), ::toupper);
		return result;
	}

	string string::trim() const
	{
		return rtrim().ltrim();
	}

	string string::trim(const char t_char) const
	{
		return rtrim(t_char).ltrim(t_char); 
	}

	string string::ltrim() const
	{
		string result(*this);
		auto it2 = std::find_if(
			result.begin(),
			result.end(),
			[](char ch)
			{ return !std::isspace(ch); }
		);
		result.erase(result.begin(), it2);
		return result;
	}

	string string::ltrim(const char t_char) const
	{
		string result(*this);
		auto it2 = std::find_if(
			result.begin(),
			result.end(),
			[t_char](char ch)
			{ return ch != t_char; }
		);
		result.erase(result.begin(), it2);
		return result;
	}

	string string::rtrim() const
	{
		string result(*this);
		auto it1 = std::find_if(
			result.rbegin(),
			result.rend(),
			[](char ch)
			{ return !std::isspace(ch); }
		);
		result.erase(it1.base(), result.end());
		return result;
	}

	string string::rtrim(const char t_char) const
	{
		string result(*this);
		auto it1 = std::find_if(
			result.rbegin(),
			result.rend(),
			[t_char](char ch)
			{ return ch != t_char; }
		);
		result.erase(it1.base(), result.end());
		return result;
	}

	std::vector<string> string::getLines() const
	{
		std::vector<string> lines;
		std::istringstream stream(*this);
		std::string line;
		while (std::getline(stream, line))
		{
			lines.push_back(line);
		}
		return lines;
	}
	
	string string::getFirstLine() const
	{
		std::istringstream stream(*this);
		std::string line;
		std::getline(stream, line);
		return line;
	}
}
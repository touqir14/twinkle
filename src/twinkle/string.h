#pragma once

#include <string>
#include <vector>

namespace twinkle
{
	class string : public std::string
	{
	public:

		string();
		string(const std::string& t_str);

		enum class CompareMode
		{
			CaseSensitive,
			IgnoreCase
		};

		std::vector<string> split(const char t_delimiter) const;

		bool contains(const char t_match) const;
		bool contains(const std::string& t_match) const;

		bool startsWith(const std::string& t_match,
			const CompareMode t_mode = CompareMode::CaseSensitive) const;
		bool endsWith(const std::string& t_match,
			const CompareMode t_mode = CompareMode::CaseSensitive) const;

		string toLower() const;
		string toUpper() const;

		string trim() const;
		string trim(const char t_char) const;
		string ltrim() const;
		string ltrim(const char t_char) const;
		string rtrim() const;
		string rtrim(const char t_char) const;

		// retrieve all the string lines
		std::vector<string> getLines() const;
		string getFirstLine() const;
		
	};
}
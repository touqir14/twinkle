#include "header.h"

namespace twinkle
{
	namespace http
	{
		std::string Header::to_string(const Version version)
		{
			if (version == Version::v2)
				return "HTTP/2.0";
			return "HTTP/1.1";
		}

		void Header::value(const std::string& str, Version& version)
		{
			if (str == "HTTP/2.0") version = Version::v2;
			else version = Version::v1;
		}

		std::string Header::to_string(const Connection connection)
		{
			if (connection == Connection::KeepAlive)
				return "keep-alive";
			return "close";
		}

		void Header::value(const std::string& str, Connection& connection)
		{
			connection = (str == "keep-alive") ? Connection::KeepAlive : Connection::Close;
		}

		std::string Header::to_string(const Method method)
		{
			switch (method)
			{
			case Method::Post: return "POST"; break;
				// ....
			case Method::Get:
			default:
				return "GET";
				break;
			}
		}

		void Header::value(const std::string& str, Method& method)
		{
			if (str == "GET") method = Method::Get;
			else if (str == "POST") method = Method::Post;
			// ...
		}

		std::string Header::to_string(const ContentEncoding encoding)
		{
			return std::string();
		}

		void Header::value(const std::string& str, ContentEncoding& encoding)
		{
		}
		
		std::string Header::to_string(const StatusCode code)
		{
			return std::to_string(static_cast<unsigned int>(code));
		}
		
		void Header::value(const std::string& str, StatusCode& code)
		{
			code = static_cast<Header::StatusCode>(std::atoi(str.c_str()));
		}
	}
}
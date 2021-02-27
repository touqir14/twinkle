#include "uri.h"

#include <sstream>

namespace twinkle
{
	namespace internet
	{
		Uri::Uri()
			: components()
		{

		}

		Uri::Uri(const std::string& uriString)
			: components()
		{
			// extract the schema
			const auto schemaDelimiter = uriString.find(':');
			components.schema = uriString.substr(0, schemaDelimiter);

			// extract the authority component
			std::string temp = uriString.substr(schemaDelimiter + 1, uriString.length());
			if (temp.substr(0, 2) == "//")
			{
				const auto authorityDelimiter = temp.find('/', 2);
				components.authority = Implementation::Authority::parse(temp.substr(2, authorityDelimiter - 2));

				temp = temp.substr(authorityDelimiter + 1, temp.length());
			}

			// extract the fragment
			const auto fragmentDelimiter = temp.find('#');
			if (fragmentDelimiter != std::string::npos)
			{
				components.fragment = temp.substr(fragmentDelimiter + 1, temp.length());

				temp = temp.substr(0, fragmentDelimiter);
			}

			// extract the query
			const auto queryDelimiter = temp.find('?');
			if (queryDelimiter != std::string::npos)
			{
				std::string query = temp.substr(queryDelimiter + 1, temp.length());
				for (const std::string& part : split(query, '&'))
				{
					const auto& pair = split(part, '=');
					if (pair.size() == 2)
					{
						components.query.insert({ pair[0], pair[1] });
					}
				}

				temp = temp.substr(0, queryDelimiter);
			}

			// extract the path
			components.path = split(temp, '/');
		}

		Uri::Uri(const Uri& other)
		{
			*this = other;
		}

		bool Uri::isValid() const
		{
			return components.schema.length() > 0;
		}

		bool Uri::hasHost() const
		{
			return components.authority.host.length() > 0;
		}

		bool Uri::hasPassword() const
		{
			return components.authority.userinfo.password.length() > 0;
		}

		bool Uri::hasPort() const
		{
			return components.authority.port != 0;
		}

		std::string Uri::toString() const
		{
			std::string result = components.schema + ":";

			const std::string authority = components.authority.toString();
			if (authority.length() > 0)
				result += ("//" + authority + "/");
			
			std::string comma;
			for (const std::string path : components.path)
			{
				result += (comma + path);
				comma = "/";
			}

			if (components.query.size() > 0)
				result += "?";
			comma = "";
			for (const auto& pair : components.query)
			{
				result += (pair.first + "=" + pair.second);
				comma = "&";
			}

			if (components.fragment.length() > 0)
				result += ("#" + components.fragment);

			return result;
		}

		Uri& Uri::operator= (const Uri& other)
		{
			components = other.components;

			return *this;
		}

		Uri& Uri::operator= (const std::string& uri)
		{			
			return *this = Uri(uri);
		}

		bool Uri::operator== (const Uri& other) const
		{
			return components == other.components;
		}

		bool Uri::operator==(const std::string& other) const
		{
			return toString() == other;
		}

		bool Uri::operator!= (const Uri& other) const
		{
			return components != other.components;
		}

		std::vector<std::string> Uri::split(const std::string& str, const char delimiter)
		{
			std::vector<std::string> tokens;
			std::string token;
			std::istringstream tokenStream(str);
			while (std::getline(tokenStream, token, delimiter))
			{
				tokens.push_back(token);
			}
			return tokens;
		}
		
		bool Uri::Implementation::operator==(const Implementation& other) const
		{
			return schema == other.schema
				&& authority == other.authority
				&& path == other.path
				&& query == other.query
				&& fragment == other.fragment;
		}
		
		bool Uri::Implementation::operator!=(const Implementation& other) const
		{
			return schema != other.schema
				|| authority != other.authority
				|| path != other.path
				|| query != other.query
				|| fragment != other.fragment;
		}
		
		bool Uri::Implementation::Authority::operator==(const Authority& other) const
		{
			return userinfo == other.userinfo
				&& host == other.host
				&& port == other.port;
		}
		
		bool Uri::Implementation::Authority::operator!=(const Authority& other) const
		{
			return userinfo != other.userinfo
				|| host != other.host
				|| port != other.port;
		}

		std::string Uri::Implementation::Authority::toString() const
		{
			std::string result = userinfo.toString();

			if (result.length() > 0)
				result += "@";
			result += host;
			if (port > 0)
				result += (":" + std::to_string(port));

			return result;
		}

		Uri::Implementation::Authority Uri::Implementation::Authority::parse(const std::string& source)
		{
			Authority authority;

			std::string temp = source;
			const auto userinfoDelimiter = source.find('@');
			if (userinfoDelimiter != std::string::npos)
			{
				authority.userinfo = Userinfo::parse(source.substr(0, userinfoDelimiter));
			}
			else
			{
				temp = source.substr(userinfoDelimiter + 1, source.length());
			}

			const auto hostDelimiter = source.find(':');
			if (hostDelimiter != std::string::npos)
			{
				authority.host = temp.substr(0, hostDelimiter);
				authority.port = std::atoi(source.substr(hostDelimiter + 1, source.length()).c_str());
			}
			else
			{
				authority.host = temp;
			}			

			return authority;
		}
		
		bool Uri::Implementation::Authority::Userinfo::operator==(const Userinfo& other) const
		{
			return username == other.username
				&& password == other.password;
		}
		
		bool Uri::Implementation::Authority::Userinfo::operator!=(const Userinfo& other) const
		{
			return username != other.username
				|| password != other.password;
		}

		std::string Uri::Implementation::Authority::Userinfo::toString() const
		{
			std::string result = username;

			if (password.length() > 0)
				result += (":" + password);

			return result;
		}
		
		Uri::Implementation::Authority::Userinfo Uri::Implementation::Authority::Userinfo::parse(const std::string& source)
		{
			Userinfo userinfo;

			const auto delimiter = source.find(':');
			if (delimiter != std::string::npos)
			{
				userinfo.username = source.substr(0, delimiter);
				userinfo.password = source.substr(delimiter + 1, source.length());
			}
			else
			{
				userinfo.username = source;
			}

			return userinfo;
		}
	}
}
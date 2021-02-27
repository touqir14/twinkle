#pragma once

#include <string>
#include <unordered_map>
#include "message.h"
#include "../string.h"

namespace twinkle
{
	namespace internet
	{
		template <typename T>
		std::string serialize(const T&) { return {}; }

		template <>
		std::string serialize(const std::unordered_map<std::string, std::string>& headers)
		{
			std::string result;
			for (auto it = headers.begin(); it != headers.end(); ++it)
			{
				result += (it->first + ": " + it->second + "\n");
			}
			return result;
		}

		template <>
		std::string serialize(const std::string& body)
		{
			return body;
		}

		template <typename Headers, typename Body>
		std::string serialize (const message_t<Headers, Body>& message)
		{
			return serialize<Headers>(message.headers)
				+ "\r\n"
				+ serialize<Body>(message.body); 
		}

		template <typename Headers, typename Body>
		bool deserialize (message_t<Headers, Body>& message, const std::string& source)
		{
			const auto it = source.find("\r\n");
			if (it != std::string::npos)
			{
				return deserialize(message.headers, source.substr(0, it))
					&& deserialize(message.body, source.substr(it + 1, source.length()));
			}
			else
			{
				return deserialize(message.headers, source)
					&& (message.body = Body{}, true); // comma operator
			}
		}

		template <typename T>
		bool deserialize(T&, const std::string&) { return false; }

		template <>
		bool deserialize(std::unordered_map<std::string, std::string>& headers, const std::string& source)
		{
			headers.clear();
			for (const auto& line : twinkle::string(source).getLines())
			{
				const auto& position = line.find(':');
				if (position != std::string::npos)
				{
					headers.insert({
						line.substr(0, position),
						line.substr(position + 1, line.length())
						});
				}
			}
			return true;
		}

		template <>
		bool deserialize(std::string& body, const std::string& source)
		{
			body = source;
			return true;
		}

		template <typename Headers, typename Body>
		std::string message_t<Headers, Body>::toString() const
		{
			return serialize(*this);
		}

		template <typename Headers, typename Body>
		message_t<Headers, Body> message_t<Headers, Body>::parse(const std::string& source)
		{
			message_t<Headers, Body> message;
			deserialize(message, source);
			return message;
		}
	}
}
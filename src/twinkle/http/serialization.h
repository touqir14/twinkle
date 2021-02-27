#pragma once

#include "../internet/serialization.h"
#include "message.h"

namespace twinkle
{
	namespace internet
	{
		template <>
		std::string serialize(const http::Headers& headers)
		{
			return serialize(static_cast<std::unordered_map<std::string, std::string>>(headers));
		}

		template <>
		bool deserialize(http::Headers& headers, const std::string& source)
		{
			return deserialize(static_cast<std::unordered_map<std::string, std::string>&>(headers), source);
		}
	}
}
#pragma once

#include <string>
#include <unordered_map>
#include "header.h"

namespace twinkle
{
	namespace http
	{
		class Headers : public std::unordered_map<std::string, std::string>
		{
		public:

			template <typename T>
			bool contains() const
			{
				return contains(Header::name<T>());
			}

			bool contains(const std::string& field) const;
			
			template <typename T>
			void set(const std::string& value)
			{
				const std::string& name = Header::name<T>();
				const auto it = find(name);
				if (it != end())
				{
					it->second = value;
				}
				else
				{
					insert({ name, value });
				}
			}

			template <typename T>
			void set(const T value)
			{
				set<T>(Header::to_string(value));
			}

			template <typename T>
			std::string value() const
			{
				const auto it = find(Header::name<T>());
				if (it != end())
					return it->second;
				return {};
			}

		private:

		};
	}
}
#include "headers.h"

namespace twinkle
{
	namespace http
	{
		bool Headers::contains(const std::string& field) const
		{
			return find(field) != end();
		}
	}
}
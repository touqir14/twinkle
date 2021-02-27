#pragma once

/*
 * Uniform Resource Locator
 *
 * Implements RFC 3986, "Uniform Resource Locator (URI): Generic Syntax"
 * https://www.ietf.org/rfc/rfc3986.txt
 */

#include "../internet/uri.h"

namespace twinkle
{
	namespace http
	{
		class Url : public internet::Uri
		{
		public:

			Url();
			Url(const std::string& uriString);
			Url(const Uri& other);

			static constexpr char * schema = "http";
		};
	}
}
#include "url.h"

namespace twinkle
{
	namespace http
	{
		Url::Url()
			: Uri()
		{
		}

		Url::Url(const std::string& uriString)
			: Uri(uriString)
		{
			components.schema = schema;
		}

		Url::Url(const Uri& other)
			: Uri(other)
		{
			components.schema = schema;
		}
	}
}
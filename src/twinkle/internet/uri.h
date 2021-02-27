#pragma once

/* 
 * This Uri class is used to parse URIs from strings,
 * render URIs as strings,
 * and get or set individual components of a URIs
 *
 * Implements RFC 3986, "Uniform Resource Identifier (URI): Generic Syntax"
 * https://www.ietf.org/rfc/rfc3986.txt
 *
 * The generic URI syntax consists of a hierarchical sequence of
 * components referred to as the scheme, authority, path, query, and
 * fragment.
 *
 * The following are two example URIs and their component parts:
 *
 *       foo://example.com:8042/over/there?name=ferret#nose
 *       \_/   \______________/\_________/ \_________/ \__/
 *        |           |            |            |        |
 *     scheme     authority       path        query   fragment
 *        |   _____________________|__
 *       / \ /                        \
 *       urn:example:animal:ferret:nose
 */

#include <string>
#include <vector>
#include <unordered_map>

namespace twinkle
{
	namespace internet
	{
		class Uri
		{
		public:

			Uri();
			Uri(const std::string& uriString);
			Uri(const Uri& other);

			bool isValid() const;

			std::string getSchema() const { return components.schema; }
			std::string getHost() const { return components.authority.host; }
			uint16_t getPort() const { return components.authority.port; }
			const std::vector<std::string>& getPath() const { return components.path; }
			const std::unordered_map<std::string, std::string>& getQuery() const { return components.query; }
			std::string getFragment() const { return components.fragment; }

			bool hasHost() const;
			bool hasPassword() const;
			bool hasPort() const;

			std::string toString() const;

			Uri& operator= (const Uri& other);
			Uri& operator= (const std::string& uri);
			bool operator== (const Uri& other) const;
			bool operator== (const std::string& other) const;
			bool operator!= (const Uri& other) const;

			struct Implementation
			{
				std::string schema;

				struct Authority
				{
					struct Userinfo
					{
						std::string username;
						std::string password;

						bool operator== (const Userinfo& other) const;
						bool operator!= (const Userinfo& other) const;
						std::string toString() const;

						static Userinfo parse(const std::string& source);

					} userinfo;

					std::string host;
					uint16_t port;

					bool operator== (const Authority& other) const;
					bool operator!= (const Authority& other) const;
					std::string toString() const;

					static Authority parse(const std::string& source);

				} authority;

				std::vector<std::string> path;
				std::unordered_map<std::string, std::string> query;
				std::string fragment;

				bool operator== (const Implementation& other) const;
				bool operator!= (const Implementation& other) const;

			} components;

		private:

			static std::vector<std::string> split(const std::string& str, const char delimiter);

		};
	}
}
#pragma once

#include <cstddef>
#include <string>

namespace twinkle
{
	namespace http
	{
		struct Header
		{
			Header() = delete;
			
			enum class AcceptCharset {};

			enum class Connection
			{
				Close,
				KeepAlive
			};

			enum class CacheControl
			{
				NoCache
			};

			enum class ContentEncoding {};

			enum class ContentLanguage {};

			enum class ContentLength {};

			enum class Date {};

			enum class Host {};

			enum class Method
			{
				Get,
				Head,
				Post,
				Put,
				Delete,
				Connect,
				Options,
				Trace
			};

			enum class StatusCode : unsigned int
			{
				Unknown = 0,
				Continue = 100,
				SwitchingProtocols = 101,
				Ok = 200,
				Created = 201,
				Accepted = 202,
				NonAuthoritativeInformation = 203,
				NoContent = 204,
				ResetContent = 205,
				MultipleChoices = 300,
				MovedPermanently = 301,
				Found = 302,
				SeeOther = 303,
				NotModified = 304,
				UseProxy = 305,
				TemporaryRedirect = 307,
				BadRequest = 400,
				Unauthorized = 401,
				PaymentRequired = 402,
				Forbidden = 403,
				NotFound = 404,
				MethodNotAllowed = 405,
				NotAcceptable = 406,
				RequestTimeout = 408,
				Conflict = 409,
				Gone = 410,
				LengthRequired = 411,
				PreconditionFailed = 412,
				PayloadTooLarge = 413,
				UriTooLong = 414,
				UnsupportedMediaType = 415,
				ExpectationFailed = 417,
				UpgradeRequired = 426,
				InternalServerError = 500,
				NotImplemented = 501,
				BadGateway = 502,
				ServiceUnavailable = 503,
				GatewayTimeout = 504,
				HttpVersionNotSupported = 505
			};

			enum class StatusCodeCategory : uint8_t
			{
				Informational = 1,
				Success = 2,
				Redirection = 3,
				ClientError = 4,
				ServerError = 5
			};

			enum class Version : uint8_t
			{
				v1 = 1,
				v2 = 2
			};

			template <typename T> static std::string name() { return {}; }
			template<> static std::string name<AcceptCharset>() { return "AcceptCharset"; }
			template<> static std::string name<Connection>() { return "Connection"; }
			template<> static std::string name<CacheControl>() { return "Cache-Control"; }
			template<> static std::string name<ContentEncoding>() { return "Encoding"; }
			template<> static std::string name<ContentLanguage>() { return "Content-Language"; }
			template<> static std::string name<ContentLength>() { return "Content-Length"; }
			template<> static std::string name<Date>() { return "Date"; }
			template<> static std::string name<Host>() { return "Host"; }
			
			static std::string to_string(const Connection connection);
			static void value(const std::string& str, Connection& connection);

			static std::string to_string(const Method method);
			static void value(const std::string& str, Method& method);

			static std::string to_string(const ContentEncoding encoding);
			static void value(const std::string& str, ContentEncoding& encoding);

			static std::string to_string(const StatusCode code);
			static void value(const std::string& str, StatusCode& code);

			static std::string to_string(const Version version);
			static void value(const std::string& str, Version& version);
		};
	}
}
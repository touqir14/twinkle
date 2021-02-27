#include "address.h"

namespace twinkle
{
	Address::Address() : m_protocol(NetworkProtocol::Unknown)
	{

	}

	Address::Address(const std::string& ip, const port_t port, const NetworkProtocol protocol)
		: m_protocol(protocol)
	{
		initialize(ip, port, protocol);
	}

	Address::Address(const std::string& address, const NetworkProtocol protocol)
		: m_protocol(protocol)
	{
		const auto separator = address.find_last_of(':');

		assert(separator != std::string::npos && "address is not of address:port form");
		assert(separator != address.size() - 1 && "address has ':' as last character. Expected port number here");

		// isolate address
		const std::string& ip = address.substr(0, separator);

		// std::stoul converts string to unsigned integer
		const auto parsed_port = std::stoul(address.substr(separator + 1).c_str(), nullptr, 10);
		// check if it is a valid port, (0, 65535) range
		assert(parsed_port < (1 << 16) && "invalid port number");

		const port_t port = static_cast<port_t>(parsed_port);

		initialize(ip, port, protocol);
	}

	Address::Address(const native_addr_t & address)
		: m_address(address)
		, m_protocol(NetworkProtocol::Unknown)
	{
		// #todo to be implemented
		m_protocol = NetworkProtocol::Unknown;
	}

	Address::port_t Address::getPort() const
	{
		if (m_protocol == NetworkProtocol::IPv4)
		{
			const sockaddr_in* addr_in = reinterpret_cast<const sockaddr_in*>(&m_address);
			return ntohs(addr_in->sin_port);
		}
		else if(m_protocol == NetworkProtocol::IPv6)
		{
			const sockaddr_in6* addr_in = reinterpret_cast<const sockaddr_in6*>(&m_address);
			return ntohs(addr_in->sin6_port);
		}
		else
		{
			return port_t();
		}
	}

	std::string Address::getIP() const
	{
		// #todo: to be implemented
		return std::string();
	}

	bool Address::isValid() const
	{	
		// #todo: to be implemented
		return m_protocol != NetworkProtocol::Unknown;
	}

	bool Address::operator== (const Address& other) const
	{
		// #todo: to be implemented
		return m_protocol == other.m_protocol;
	}

	bool Address::operator!= (const Address& other) const
	{
		// #todo: to be implemented
		return m_protocol != other.m_protocol;
	}

	const std::string& Address::toString() const
	{
		return getIP() + ":" + std::to_string(getPort());
	}

	void Address::initialize(const std::string& ip, const port_t port, const NetworkProtocol protocol)
	{
		const network_proto_t native_protocol = NetworkProtocolInfo::resolve(protocol);

		if (m_protocol == NetworkProtocol::IPv4)
		{
			sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(&m_address);
			addr_in->sin_family = native_protocol;
			addr_in->sin_port = htons(port);
#if PLATFORM_WINDOWS
			InetPton(native_protocol, ip.c_str(), &addr_in->sin_addr);
#else
			inet_pton(native_protocol, ip.c_str(), &addr_in->sin_addr);
#endif
		}
		else if(m_protocol == NetworkProtocol::IPv6)
		{
			// #todo: to be implemented
		}
	}

	Address::network_proto_t Address::NetworkProtocolInfo::resolve(const NetworkProtocol protocol)
	{
		if (protocol == NetworkProtocol::IPv4)
			return AF_INET;
		else if (protocol == NetworkProtocol::IPv6)
			return AF_INET6;
		else // Unknown
			return AF_UNSPEC;
	}

	Address::NetworkProtocol Address::NetworkProtocolInfo::resolve(const network_proto_t protocol)
	{
		if (protocol == AF_INET)
			return NetworkProtocol::IPv4;
		else if (protocol == AF_INET6)
			return NetworkProtocol::IPv6;
		else // Unknown
			return NetworkProtocol::Unknown;
	}
}
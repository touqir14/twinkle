#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include "common.h"

namespace twinkle
{
	class Address
	{
	public:

		// identify the network protocol
		enum class NetworkProtocol
		{
			Unknown,
			IPv4,
			IPv6
		};

		using network_proto_t = uint8_t;

		struct NetworkProtocolInfo
		{
			static network_proto_t resolve(const NetworkProtocol protocol);
			static NetworkProtocol resolve(const network_proto_t protocol);
		};

		// native address type
		using native_addr_t = sockaddr;
		// port type 
		using port_t = uint16_t;

		Address();
		Address(const std::string& ip, const port_t port, const NetworkProtocol protocol = NetworkProtocol::IPv4);
		Address(const std::string& address, const NetworkProtocol protocol = NetworkProtocol::IPv4);
		Address(const native_addr_t& address);
		~Address() = default;

		inline const native_addr_t& getNativeAddress() const { return m_address; }
		inline std::size_t getNativeSize() const { return sizeof(m_address); }
		inline NetworkProtocol getProtocol() const { return m_protocol; }

		port_t getPort() const;
		std::string getIP() const;

		bool isValid() const;
		inline operator bool() const { return isValid(); }

		inline bool operator== (const Address& other) const;
		inline bool operator!= (const Address& other) const;

		const std::string& toString() const;

	protected:

		// native address type
		native_addr_t m_address{};
		// network protocol
		NetworkProtocol m_protocol;

	private:

		void initialize(const std::string& ip, const port_t port, const NetworkProtocol protocol);
	};
}
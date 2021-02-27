#pragma once

#include "address.h"
#include "platform.h"

namespace twinkle
{
	class Socket
	{
	public:

		// transport protocol
		enum class TransportProtocol
		{
			Unknown,
			TCP,
			UDP
		};

		using transport_proto_t = uint8_t;

		struct TransportProtocolInfo
		{
			static transport_proto_t resolve(const TransportProtocol protocol);
			static TransportProtocol resolve(const transport_proto_t protocol);
		};

		// native socket type
		using native_socket_t =
#ifdef PLATFORM_WINDOWS
		SOCKET
#else 
		int
#endif
		;

		// indicates the type of socket being used
		enum class Type
		{
			Unknown,
			// UDP type socket
			Datagram,
			// TCP type socket 
			Stream
		};

		using type_t = uint8_t;

		struct TypeInfo
		{
			static type_t resolve(const Type type);
			static Type resolve(const type_t type);
		};

		// indicates the connection state of the socket
		enum class ConnectionState
		{
			NotConnected,
			Connected,
			Error
		};

		enum class ShutdownMode
		{
			// disable reading on the socket
			Read,
			// disable writing on the socket
			Write,
			// disable reading and writing on the socket
			Both
		};

		Socket(const Address& address, const TransportProtocol protocol);
		Socket(const native_socket_t socket, const Address& address, const TransportProtocol protocol);
		~Socket() = default;

		inline TransportProtocol getProtocol() const { return m_protocol; }
		inline const Address& getAddress() const { return m_address; }

		// Notify the operating system that a socket will use a specific address
		// and transport layer port.
		bool bind();
		// Calling connect initialized the TCP handshake by sending 
		// the initial SYN packet to a target host.
		// if the host has a listen socket bound to the appropriate port, 
		// it can proceed with the handshake by calling accept.
		bool connect(const Address& address);
		// numOfMaxConnections is the maximum number of incoming 
		// connections that should be allowed to queue up.
		// Once the maximum number of handshakes are pending, 
		// any futher incoming connection is dropped.
		bool listen(unsigned int numOfMaxConnections);
		// accept an incoming connection and continue to the handshake.
		Socket* accept() const;

		// A connected socket stores the remote host's address information.
		// Because of this, a process does not need to pass an address. 
		// @param data The buffer to send
		// @param count The size of the data to send
		// @param byteSent Will indicate how much was sent
		bool send(const uint8_t* data, const std::size_t count, int32_t& byteSent);
		// Send data to a specific address
		bool sendTo(const Address& address, const uint8_t* data, const std::size_t count, int32_t& byteSent);

		bool receive(uint8_t* data, std::size_t bufferSize, int32_t& bytesRead);
		bool receiveFrom(const Address& address, uint8_t* data, std::size_t bufferSize, int32_t& bytesRead);

		bool setNonBlockingMode(const bool isNonBlocking = true);
		bool setBroadcastMode(const bool isBroadcast = true);

		// shuts down the socket
		bool shutdown(ShutdownMode mode = ShutdownMode::Both);
		// close the socket
		bool close();

		bool isValid() const;
		inline operator bool() const { return isValid(); }

	protected:

		// native socket type
		native_socket_t m_socket{};
		// socket address
		Address m_address;
		// socket type
		Type m_type;
		// transport protocol
		TransportProtocol m_protocol;
	};
}
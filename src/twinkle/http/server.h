#pragma once

#include <map>
#include <thread>
#include "../socket.h"

namespace twinkle
{
	namespace http
	{
		class Server
		{
		public:

			Server(const Address& address, const unsigned int maxConnections);

			// retrieve the socket
			Socket* const getSocket() const { return m_socket; }

			bool listen();
			void close();

		private:

			void handleConnections();
			void handleClientRequests(Socket* const clientSocket);

			bool m_isRunning;
			// server socket
			Socket* m_socket;
			// max connections the server can manage
			unsigned int m_maxConnections;
			// clients accept thread
			std::thread m_acceptThread;
			// clients collection
			std::map<Socket*, std::thread*> m_clients;
		};
	}
}
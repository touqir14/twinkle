#include "server.h"

#include <iostream>
#include "message.h"

namespace twinkle
{
	namespace http
	{
		Server::Server(const Address& address, const unsigned int maxConnections)
			: m_isRunning(false)
			, m_socket(new Socket(address, Socket::TransportProtocol::TCP))
			, m_maxConnections(maxConnections)
			, m_acceptThread(&Server::handleConnections, this)
			, m_clients()
		{

		}

		bool Server::listen()
		{
			if (m_socket->bind() && m_socket->listen(m_maxConnections))
			{
				m_isRunning = true;
				m_acceptThread.join();
			}
			return m_isRunning;
		}

		void Server::close()
		{
			if (m_isRunning)
			{
				m_isRunning = false;
				m_acceptThread.detach();

				for (const auto& pair : m_clients)
				{
					pair.second->detach();
				}
			}
		}

		void Server::handleConnections()
		{
			do
			{
				if (Socket* clientSocket = m_socket->accept())
				{
					std::thread* clientThread = new std::thread(
						&Server::handleClientRequests,
						this,
						clientSocket
					);
					m_clients.insert({ clientSocket, clientThread });
					clientThread->join();
				}
			} while (m_isRunning);
		}

		void Server::handleClientRequests(Socket* const clientSocket)
		{
			char buffer[3000];
			const std::size_t buffer_size = sizeof(buffer);
			int32_t byteRead{ 0 };

			do
			{
				if (clientSocket->receive(
					(uint8_t*)(buffer),
					buffer_size,
					byteRead
				))
				{
					std::string data = { buffer, (unsigned int)byteRead };

					Request request = Request::parse(data);
					std::cout << request.toString();

					Response response;
					response.body = "<h1>Hello twinkle!</h1>";
					response.headers.set(Header::Connection::Close);
					response.description = "OK";
					const std::string& welcome_message = response.toString();

					int32_t byteSent;
					clientSocket->send(
						reinterpret_cast<const uint8_t*>(welcome_message.c_str()),
						welcome_message.size(),
						byteSent
					);
				}
			} while (m_isRunning);
		}
	}
}
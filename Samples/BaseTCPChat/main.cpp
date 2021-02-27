#include <iostream>
#include <twinkle/twinkle.h>

using namespace std;
using namespace twinkle;

void client_main();
void server_main();

#define SERVER_PORT 9670
#define CLIENT_PORT 9000

int main()
{
	twinkle::startup();

	int choice = 0;
	cout << "TCP Chat [0: Server, 1: Client]: ";
	cin >> choice;

	if (choice == 0)
		server_main();
	else client_main();

	cout << "Premi un tasto per continuare...";
	int a;
	cin >> a;

	twinkle::shutdown();

	return 0;
}

void client_main()
{
	Socket sock({ "127.0.0.1", CLIENT_PORT }, Socket::TransportProtocol::TCP);
	if (sock.isValid())
	{
		if (sock.connect({ "127.0.0.1", SERVER_PORT }))
		{
			cout << "Socket connected successfully!" << endl;
			
			char buffer[3000];
			const std::size_t buffer_size = sizeof(buffer);
			int32_t byteRead{ 0 };

			if (sock.receive((uint8_t*)buffer, buffer_size, byteRead))
			{
				cout << "Message: " << std::string{ buffer, (unsigned int)byteRead } << endl;
			}
			else cout << "Unable to receive a message" << endl;
		}
		else
		{
			cout << "Invalid socket connection: " << endl;
		}
	}
	else
	{
		cout << "Invalid socket creation: " << endl;
	}
}

void server_main()
{
	Socket sock({ "127.0.0.1", SERVER_PORT }, Socket::TransportProtocol::TCP);
	if (sock.isValid())
	{
		if (sock.bind())
		{
			if (sock.listen(20))
			{
				cout << "Socket listening successfully!" << endl;

				if (Socket* clientSocket = sock.accept())
				{
					cout << "Press to send a message...";
					getchar();

					std::string message = "ciao";
					int byteSent = 0;

					if (clientSocket->send(
						(uint8_t*)message.c_str(),
						message.size(),
						byteSent
					))
					{
						cout << "Message sent!" << endl;
					}
					else cout << "Unable to send the message" << endl;
				}
				else cout << "Unable to accept client's socket" << endl;
			}
			else cout << "Unable to listen..." << endl;
		}
		else
		{
			cout << "Invalid socket binding: " << endl;
		}
	}
	else
	{
		cout << "Invalid socket creation: " << endl;
	}
}
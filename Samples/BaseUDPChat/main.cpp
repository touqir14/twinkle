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
	cout << "UDP Chat [0: Server, 1: Client]: ";
	cin >> choice;

	if (choice == 0)
		server_main();
	else client_main();

	cout << "Premi un tasto per continuare...";
	getchar();

	twinkle::shutdown();

	return 0;
}

void client_main()
{
	UDPSocket sock({ Address::localAddress, CLIENT_PORT });
	if (sock.isValid())
	{
		if (sock.bind())
		{
			cout << "Socket binded successfully!" << endl;
			std::string message{};
			Address fromAddress{};
			if (sock.receive(message, fromAddress))
			{
				cout << "Message: " << message << endl;
			}
			else cout << "Unable to receive a message" << endl;
		}
		else
		{
			cout << "Invalid socket binding: " << twinkle::getErrorMessage() << endl;
		}
	}
	else
	{
		cout << "Invalid socket creation: " << twinkle::getErrorMessage() << endl;
	}
}

void server_main() 
{
	UDPSocket sock({ Address::localAddress, SERVER_PORT });
	if (sock.isValid())
	{
		if (sock.bind())
		{
			cout << "Socket binded successfully!" << endl;
			cout << "Press to send a message...";
			getchar();
			if (sock.send("ciao mondo", { Address::localAddress, CLIENT_PORT }))
			{
				cout << "Message sent!" << endl;
			}
			else cout << "Unable to send the message" << endl;
		}
		else
		{
			cout << "Invalid socket binding: " << twinkle::getErrorMessage() << endl;
		}
	}
	else
	{
		cout << "Invalid socket creation: " << twinkle::getErrorMessage() << endl;
	}
}
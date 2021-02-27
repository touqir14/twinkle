#include <iostream>
#include <twinkle/twinkle.h>

using namespace std;
using namespace twinkle;

int main()
{
	twinkle::startup();

	Address address("127.0.0.1:9090");
	Socket socket(address, Socket::TransportProtocol::TCP);

	uint8_t buffer[100];
	const std::size_t buffer_size = sizeof(buffer);
	int32_t byteRead{ 0 };
	int32_t byteSent{ 0 };

	if (socket.bind() && socket.listen(20))
	{
		cout << "Listening..." << endl;
	}

	Socket* client = nullptr;

	while (socket.isValid())
	{
		if (!client)
		{
			if (client = socket.accept())
			{
				cout << "Connected" << endl;
				std::string welcome_text = "ciao";
				socket.send((uint8_t*)welcome_text.c_str(), welcome_text.size(), byteSent);
			}
		}

		if (socket.receive(buffer, buffer_size, byteRead))
		{
			std::string message{ reinterpret_cast<char*>(buffer), (unsigned int)byteRead };
			cout << message << endl;
		}
	}

	twinkle::shutdown();

	return 0;
}
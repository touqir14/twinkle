#pragma once

#include <cerrno>
#include <functional>
#include <iostream>
#include <string>

namespace twinkle
{
	// override this function to define custom log behaviour
	std::function<void(const std::string&)> log = [](const std::string& text)
	{
		std::cout << text << std::endl;
	};

	std::string getErrorMessage()
	{
		const size_t bufferSize = FILENAME_MAX;
		char buffer[FILENAME_MAX];
#if WIN32
		strerror_s(buffer, bufferSize, WSAGetLastError());
#else 
		//strerror_s(buffer, bufferSize, errno);
#endif
		return std::string{ buffer };
	}
}
#include "Console.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Console
{
	//
	// Private
	//

	void Console::readString(std::string& p_string) 
	{
	#ifdef _WIN32
		// The only way to read unicode characters from the console on Windows is to read UTF-16 in a wide string.
		wchar_t buffer[1 << 10]; // We don't know before how much is going to be read, so we use a large enough buffer (1kb).
		DWORD bufferSize;
		ReadConsoleW(m_inputHandle, buffer, std::size(buffer), &bufferSize, nullptr);
		bufferSize -= 2; // remove \r\n return characters

		// Because we use utf-8, convert it from UTF-16 to UTF-8 and store that in the output.
		auto outputSize = WideCharToMultiByte(CP_UTF8, 0, buffer, bufferSize, 0, 0, 0, 0);
		p_string.resize(outputSize);
		WideCharToMultiByte(CP_UTF8, 0, buffer, bufferSize, (char*)p_string.data(), outputSize, 0, 0);
	#else
		// Most other platforms use UTF-8 by default.
		std::cin >> p_string;
	#endif
	}

	//
	// Public
	//

	Console::Console()
	{
	#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8);
		m_inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	#endif
	}
}

#include "unicode_console.hpp"

#ifdef _WIN32
#	include <Windows.h>
#endif

namespace unicode_console {

Console::Console() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	input_handle_ = GetStdHandle(STD_INPUT_HANDLE);
#endif
}

void Console::read_string_(std::string& output_string) {
#ifdef _WIN32
	// The only way to read unicode characters from the console on Windows is to read UTF-16 in a wide string.
	std::array<wchar_t, 1 << 10> buffer; // We don't know before how much is going to be read, so we use a large enough buffer (1kb).
	DWORD input_size;
	ReadConsoleW(input_handle_, buffer.data(), buffer.size(), &input_size, nullptr);
	input_size -= 2; // remove \r\n return characters

	// Because we use utf-8, convert it from UTF-16 to UTF-8 and store that in the output.
	auto output_size = WideCharToMultiByte(CP_UTF8, 0, buffer, input_size, 0, 0, 0, 0);
	output_string.resize(output_size);
	WideCharToMultiByte(CP_UTF8, 0, buffer, input_size, output_string.data(), output_size, 0, 0);
#else
	// Most other platforms use UTF-8 by default.
	std::cin >> output_string;
#endif
}

} // namespace unicode_console

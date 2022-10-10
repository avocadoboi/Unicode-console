#pragma once

#include <iostream>
#include <limits>
#include <string_view>

namespace unicode_console {

/*
	A console i/o class that supports UTF-8 (8-bit unicode) input and output.
	This allows the usage of all characters that are supported by the console font.
	It also provides a simpler interface and better error handling.
*/
class Console {
public:
	/*
		This is the same as std::cout.operator<<.
	*/
	template<typename T>
	Console& operator<<(T const& type) {
		std::cout << type;
		return *this;
	}
	/*
		Prints arguments to the console and appends a new line afterwards.
	*/
	template<typename ... Argument>
	Console& println(Argument&& ... arguments) {
		(std::cout << ... << std::forward<Argument>(arguments)) << '\n';
		return *this;
	}

	/*
		Returns whether the last input read from the console was the correct type.
	*/
	bool was_last_input_valid() const {
		return was_last_input_valid_;
	}
	/*
		Same as was_last_input_valid().
	*/
	operator bool() const {
		return was_last_input_valid_;
	}
	/*
		Same as !was_last_input_valid().
	*/
	bool operator!() const {
		return !was_last_input_valid_;
	}

	/*
		This uses std::cin.operator>> but clears newline characters if user wrote invalid input.
		If the user did provide invalid input, was_last_input_valid() or operator bool() will 
		return false until the next time input is correctly read.
	*/
	template<typename T>
	Console& operator>>(T& type) {
		std::cin >> type;
		was_last_input_valid_ = static_cast<bool>(std::cin);
		if (!was_last_input_valid_) {
			// A number was expected but the user wrote characters, so we need to ignore the 
			// trailing newline character and clear the error state of std::cin.
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		return *this;
	}

	/*
		This is the same as for example:
			auto string = std::string{};
			console >> string;
		but a simpler interface.
	*/
	template<typename T>
	T read() {
		T output;
		operator>>(output);
		return output;
	}

	/*
		Reads a line of text from the console.
	*/
	std::string readln() {
		auto result = std::string{};
		std::getline(std::cin, result);
		return result;
	}

	/*
		Reads a line of text from the console and prints error_message if get_is_valid returns
		false and tries again until get_is_valid returns true.
	*/
	template<typename ValidatorType>
	std::string readln_validated(ValidatorType const& get_is_valid, std::string_view const error_message) {
		auto result = std::string{};
		while (true) {
			std::getline(std::cin, result);

			if (get_is_valid(result)) {
				return result;
			}
			else {
				println(error_message);
			}
		}
		return result;
	}

	/*
		Reads input from the console and prints error_message if the input did not
		correspond to the datatype T and tries again until it does.
	*/
	template<typename T>
	T read_validated(std::string_view const error_message) {
		T result;
		while (true) {
			operator>>(result);
			if (operator bool()) {
				return result;
			}
			else {
				println(error_message);
			}
		}
	}
	/*
		Reads input from the console and prints error_message if get_is_valid returns
		false and tries again until get_is_valid returns true.
	*/
	template<typename T, typename ValidatorType>
	T read_validated(ValidatorType const& get_is_valid, std::string_view const error_message) {
		T result;
		while (true) {
			operator>>(result);

			if (was_last_input_valid_ && get_is_valid(result)) {
				return result;
			}
			else {
				println(error_message);
			}
		}
	}
	/*
		Reads input from the console and prints type_validation_error_message if the input did not correspond to the datatype T.
		If it was correctly read as T then it prints custom_validation_error_message if get_is_valid returns false.
		If the input was invalid in any of these two ways it tries to read input again until the input is valid.
	*/
	template<typename T, typename ValidatorType>
	T read_validated(
		ValidatorType const& get_is_valid, 
		std::string_view const custom_validation_error_message, 
		std::string_view const type_validation_error_message
	) {
		T result;
		while (true) {
			operator>>(result);

			if (operator!()) {
				println(type_validation_error_message);
			}
			else if (!get_is_valid(result)) {
				println(custom_validation_error_message);
			}
			else {
				return result;
			}
		}
	}

	Console();

private:
	void read_string_(std::string& string);

#ifdef _WIN32
	void* input_handle_; // Used to read unicode from console in the method below
#endif

	bool was_last_input_valid_ = true;
};

/*
	Specialization of the input operator template, to support UTF-8 input on Windows.
*/
template<>
inline Console& Console::operator>><std::string>(std::string& string) {
	read_string_(string);
	return *this;
}

/*
	An instance of Console.
*/
inline Console io;

/*
	Prints arguments to the console and appends a new line afterwards.
*/
template<typename ... Argument>
Console& println(Argument&& ... arguments) {
	return io.println(std::forward<Argument>(arguments)...);
}
/*
	This is the same as for example:
		auto string = std::string{};
		Console::io >> string;
	but a simpler interface.
*/
template<typename T>
T read() {
	return io.read<T>();
}

/*
	Reads a line of text from the console.
*/
inline std::string readln() {
	return io.readln();
}

/*
	Reads a line of text from the console and prints error_message if get_is_valid returns
	false and tries again until get_is_valid returns true.
*/
template<typename ValidatorType>
std::string readln_validated(ValidatorType const& get_is_valid, std::string_view const error_message) {
	return io.readln_validated(get_is_valid, error_message);
}

/*
	Reads input from the console and prints error_message if the input did not
	correspond to the datatype T and tries again until it does.
*/
template<typename T>
T read_validated(std::string_view const error_message) {
	return io.read_validated<T>(error_message);
}
/*
	Reads input from the console and prints error_message if get_is_valid returns
	false and tries again until get_is_valid returns true.
*/
template<typename T, typename ValidatorType>
T read_validated(ValidatorType const& get_is_valid, std::string_view const error_message) {
	return io.read_validated<T>(get_is_valid, error_message);
}
/*
	Reads input from the console and prints type_validation_error_message if the input did not correspond to the datatype T.
	If it was correctly read as T then it prints custom_validation_error_message if get_is_valid returns false.
	If the input was invalid in any of these two ways it tries to read input again until the input is valid.
*/
template<typename T, typename ValidatorType>
T read_validated(
	ValidatorType const& get_is_valid, 
	std::string_view const custom_validation_error_message, 
	std::string_view const type_validation_error_message
) {
	return io.read_validated<T>(get_is_valid, custom_validation_error_message, type_validation_error_message);
}

} // namespace unicode_console

#pragma once

#include <iostream>
#include <string>

/*
	A console i/o class that supports UTF-8 (8-bit unicode) input and output.
	This allows the usage of all characters that are supported by the console font.
	It also provides a simpler interface and better error handling.
*/
class Console
{
public:
	/*
		This is the same as std::cout.operator<<.
	*/
	template<typename T>
	Console const& operator<<(T const& p_type) const
	{
		std::cout << p_type;
		return *this;
	}
	/*
		Prints p_arguments to the console and appends a new line afterwards.
	*/
	template<typename ... Argument>
	Console const& println(Argument&& ... p_arguments) const
	{
		((std::cout << std::forward<Argument>(p_arguments)), ...) << '\n';
		return *this;
	}

	/*
		This uses std::cin.operator>> but clears newline characters if user wrote invalid input.
	*/
	template<typename T>
	Console const& operator>>(T& p_type) const
	{
		std::cin >> p_type;
		if (!std::cin)
		{
			// A number was expected but the user wrote characters, so we need to clear newline characters.
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		return *this;
	}

private:
#ifdef _WIN32
	void* m_inputHandle; // Used to read unicode from console in the method below
#endif
	void readString(std::string& p_string) const;

public:
	/*
		This is the same as for example:
			std::string string;
			console >> string;
		but a simpler interface.
	*/
	template<typename T>
	T read() const
	{
		T output;
		operator>>(output);
		return output;
	}

	/*
		Reads input from the console and prints p_errorMessage if p_getIsValid returns 
		false and tries again until p_getIsValid returns true.
	*/
	template<typename T, typename ValidatorType>
	T readValidated(ValidatorType const& p_getIsValid, std::string const& p_errorMessage = "") const
	{
		T result;
		while (true)
		{
			operator>>(result);

			if (p_getIsValid(result))
			{
				return result;
			}
			else
			{
				println(p_errorMessage);
			}
		}
	}

public:
	Console();
};

/*
	Specialization of the input operator template, to support UTF-8 input on Windows.
*/
template<>
inline Console const& Console::operator>><std::string>(std::string& p_string) const
{
	readString(p_string);
	return *this;
}

/*
	A global instance of Console.
*/
inline Console const console;

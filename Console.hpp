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
	Console& operator<<(T const& p_type) 
	{
		std::cout << p_type;
		return *this;
	}
	/*
		Prints p_arguments to the console and appends a new line afterwards.
	*/
	template<typename ... Argument>
	Console& println(Argument&& ... p_arguments) 
	{
		((std::cout << std::forward<Argument>(p_arguments)), ...) << '\n';
		return *this;
	}

private:
	bool m_wasLastInputValid = true;
public:
	/*
		Returns whether the last input read from the console was the correct type.
	*/
	bool getWasLastInputValid() 
	{
		return m_wasLastInputValid;
	}
	/*
		Same as getWasLastInputValid().
	*/
	operator bool() 
	{
		return m_wasLastInputValid;
	}
	/*
		Same as !getWasLastInputValid().
	*/
	bool operator!() 
	{
		return !m_wasLastInputValid;
	}

	/*
		This uses std::cin.operator>> but clears newline characters if user wrote invalid input.
	*/
	template<typename T>
	Console& operator>>(T& p_type)
	{
		std::cin >> p_type;
		m_wasLastInputValid = (bool)std::cin;
		if (!m_wasLastInputValid)
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
	void readString(std::string& p_string);

public:
	/*
		This is the same as for example:
			std::string string;
			console >> string;
		but a simpler interface.
	*/
	template<typename T>
	T read() 
	{
		T output;
		operator>>(output);
		return output;
	}

	/*
		Reads input from the console and prints p_errorMessage if the input did not 
		correspond to the datatype T and tries again until it does.
	*/
	template<typename T>
	T readValidated(std::string const& p_errorMessage) 
	{
		T result;
		while (true)
		{
			operator>>(result);
			if (operator bool())
			{
				return result;
			}
			else
			{
				println(p_errorMessage);
			}
		}
	}
	/*
		Reads input from the console and prints p_errorMessage if p_getIsValid returns 
		false and tries again until p_getIsValid returns true.
	*/
	template<typename T, typename ValidatorType>
	T readValidated(ValidatorType const& p_getIsValid, std::string const& p_errorMessage) 
	{
		T result;
		while (true)
		{
			operator>>(result);

			if (operator bool() && p_getIsValid(result))
			{
				return result;
			}
			else
			{
				println(p_errorMessage);
			}
		}
	}
	/*
		Reads input from the console and prints p_typeValidationErrorMessage if the input did not correspond to the datatype T.
		If it was correctly read as T then it prints p_customValidationErrorMessage if p_getIsValid returns false.
		If the input was invalid in any of these two ways it tries to read input again until the input is valid.
	*/
	template<typename T, typename ValidatorType>
	T readValidated(ValidatorType const& p_getIsValid, std::string const& p_customValidationErrorMessage, std::string const& p_typeValidationErrorMessage)
	{
		T result;
		while (true)
		{
			operator>>(result);

			if (operator!())
			{
				println(p_typeValidationErrorMessage);
			}
			else if (!p_getIsValid(result))
			{
				println(p_customValidationErrorMessage);
			}
			else
			{
				return result;
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
inline Console& Console::operator>><std::string>(std::string& p_string)
{
	readString(p_string);
	return *this;
}

/*
	A global instance of Console.
*/
inline Console console;

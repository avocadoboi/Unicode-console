#pragma once

#include <iostream>
#include <string_view>

namespace Console
{
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
		auto operator<<(T const& p_type) -> Console&
		{
			std::cout << p_type;
			return *this;
		}
		/*
			Prints p_arguments to the console and appends a new line afterwards.
		*/
		template<typename ... Argument>
		auto println(Argument&& ... p_arguments) -> Console&
		{
			(std::cout << ... << std::forward<Argument>(p_arguments)) << '\n';
			return *this;
		}

	private:
		bool m_wasLastInputValid = true;
	public:
		/*
			Returns whether the last input read from the console was the correct type.
		*/
		auto getWasLastInputValid() noexcept -> bool
		{
			return m_wasLastInputValid;
		}
		/*
			Same as getWasLastInputValid().
		*/
		operator bool() noexcept
		{
			return m_wasLastInputValid;
		}
		/*
			Same as !getWasLastInputValid().
		*/
		auto operator!() noexcept -> bool
		{
			return !m_wasLastInputValid;
		}

		/*
			This uses std::cin.operator>> but clears newline characters if user wrote invalid input.
			If the user did provide invalid input, getWasLastInputValid() or operator bool() will 
			return false until the next time input is correctly read.
		*/
		template<typename T>
		auto operator>>(T& p_type) -> Console&
		{
			std::cin >> p_type;
			m_wasLastInputValid = (bool)std::cin;
			if (!m_wasLastInputValid)
			{
				// A number was expected but the user wrote characters, so we need to ignore the 
				// trailing newline character and clear the error state of std::cin.
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			return *this;
		}

	private:
#ifdef _WIN32
		void* m_inputHandle; // Used to read unicode from console in the method below
#endif
		auto readString(std::string& p_string) -> void;

	public:
		/*
			This is the same as for example:
				auto string = std::string{};
				console >> string;
			but a simpler interface.
		*/
		template<typename T>
		auto read() -> T
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
		auto readValidated(std::string_view const p_errorMessage) -> T
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
		auto readValidated(ValidatorType const& p_getIsValid, std::string_view const p_errorMessage) -> T
		{
			T result;
			while (true)
			{
				operator>>(result);

				if (m_wasLastInputValid && p_getIsValid(result))
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
		auto readValidated(ValidatorType const& p_getIsValid, 
		                   std::string_view const p_customValidationErrorMessage, 
						   std::string_view const p_typeValidationErrorMessage) -> T
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
	inline auto Console::operator>><std::string>(std::string& p_string) -> Console&
	{
		readString(p_string);
		return *this;
	}

	/*
		An instance of Console.
	*/
	inline Console io;

	/*
		Prints p_arguments to the console and appends a new line afterwards.
	*/
	template<typename ... Argument>
	auto println(Argument&& ... p_arguments) -> Console&
	{
		return io.println(std::forward<Argument>(p_arguments)...);
	}
	/*
		This is the same as for example:
			auto string = std::string{};
			Console::io >> string;
		but a simpler interface.
	*/
	template<typename T>
	auto read() -> T
	{
		return io.read<T>();
	}
	/*
		Reads input from the console and prints p_errorMessage if the input did not
		correspond to the datatype T and tries again until it does.
	*/
	template<typename T>
	auto readValidated(std::string_view const p_errorMessage) -> T
	{
		return io.readValidated<T>(p_errorMessage);
	}
	/*
		Reads input from the console and prints p_errorMessage if p_getIsValid returns
		false and tries again until p_getIsValid returns true.
	*/
	template<typename T, typename ValidatorType>
	auto readValidated(ValidatorType const& p_getIsValid, std::string_view const p_errorMessage) -> T
	{
		return io.readValidated<T>(p_getIsValid, p_errorMessage);
	}
	/*
		Reads input from the console and prints p_typeValidationErrorMessage if the input did not correspond to the datatype T.
		If it was correctly read as T then it prints p_customValidationErrorMessage if p_getIsValid returns false.
		If the input was invalid in any of these two ways it tries to read input again until the input is valid.
	*/
	template<typename T, typename ValidatorType>
	auto readValidated(ValidatorType const& p_getIsValid, 
	                   std::string_view const p_customValidationErrorMessage, 
					   std::string_view const p_typeValidationErrorMessage) -> T
	{
		return io.readValidated<T>(p_getIsValid, p_customValidationErrorMessage, p_typeValidationErrorMessage);
	}
}

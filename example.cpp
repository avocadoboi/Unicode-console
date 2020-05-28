#include "Console.hpp"

int main()
{
	Console::println(u8"Write something! Maybe unicode Åå Ää Öö Ññ Üü α δ λ μ π τ")
	        .println("You wrote: \"", Console::read<std::string>(), "\".");

	Console::io << "\nNow write a number between 1 and 4: ";

	auto number = Console::readValidated<int>(
		[](int number) {
			return number >= 1 && number <= 4;
		},
		"That's outside of the range. Try again.",
		"That's not a number. Try again."
	);
	
	Console::println("Thanks, you wrote ", number, ".");

	std::string firstInput, secondInput;
	Console::io << "\nNow write something interesting: " >> firstInput 
	            << "Yes, something more please: " >> secondInput
	            << "This is what you wrote, but the other way around: " 
	            << secondInput << ' ' << firstInput << '\n';

	Console::println("How nice!");
}

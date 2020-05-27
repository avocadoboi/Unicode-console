#include "Console.hpp"

int main()
{
	console.println(u8"Write something! Maybe unicode Åå Ää Öö Ññ Üü α δ λ μ π τ");

	console.println("You wrote: \"", console.read<std::string>(), "\".");

	console << "\nNow write a number between 1 and 4: ";

	auto number = console.readValidated<int>(
		[](int number) {
			return number >= 1 && number <= 4;
		},
		"That's outside of the range. Try again.",
		"That's not a number. Try again."
	);
	
	console.println("Thanks, you wrote ", number, ".");

	std::string firstInput, secondInput;
	console << "\nNow write something interesting: " >> firstInput 
	        << "Yes, something more please: " >> secondInput
	        << "This is what you wrote, but the other way around: " 
	        << secondInput << ' ' << firstInput << '\n';

	console.println("How nice!");
}

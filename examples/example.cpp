#include <unicode_console.hpp>

int main() {
	using namespace unicode_console; 
	
	println("Write something! Maybe unicode Åå Ää Öö Ññ Üü α δ λ μ π τ");
	println("You wrote: \"", readln(), "\".");

	io << "\nNow write a number between 1 and 4: ";

	auto const number = read_validated<int>(
		[](int const number) {
			return number >= 1 && number <= 4;
		},
		"That's outside of the range. Try again.",
		"That's not a number. Try again."
	);
	
	println("Thanks, you wrote ", number, ".");

	std::string first_input, second_input;
	io << "\nNow write something interesting: " >> first_input 
		<< "Yes, something more please: " >> second_input
		<< "This is what you wrote, but the other way around: " 
	    << second_input << ' ' << first_input << '\n';
}
